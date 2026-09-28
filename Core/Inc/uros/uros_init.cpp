/*
 * uros_init.cpp
 *
 *  Created on: Jul 15, 2026
 *      Author: hsuanjung
 * 
 *  主要修正：
 *  1. MechanismCommand 內含兩個 unbounded string，必須「預先配置」緩衝區，
 *     否則 cdr_deserialize 會因 capacity 不足直接回傳 false，
 *     executor 就不會呼叫 callback（靜默失敗）。
 *  2. AGENT_CONNECTED 狀態不再每圈 ping，改成每 1 秒 ping 一次，
 *     其餘時間全部拿去 spin executor。
 */




#include "uros_init.h"
#include <math.h>
#include <string.h>
#include <rmw_microros/time_sync.h>

rcl_subscription_t                          mechanism_command_sub;
robot_interfaces__msg__MechanismCommand     mechanism_command_msg;

volatile uint16_t  mechanism_command_id = 0;
volatile bool      mechanism_command_pending = false;
volatile uint32_t  mechanism_command_rx_count = 0;

/* 除錯用：可以在 Live Expressions 直接看到收到的字串 */
char mechanism_command_name_dbg[32] = {0};

rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;
rcl_init_options_t init_options;
rclc_executor_t executor;

agent_status_t status = AGENT_WAITING;


int ping_fail_count = 0;
#define MAX_PING_FAIL_COUNT 5


/* AGENT_CONNECTED 狀態下的 ping 週期 (ms) */
#define AGENT_PING_PERIOD_MS 1000U


extern UART_HandleTypeDef huart3;

/* ------------------------------------------------------------------
 * 靜態字串緩衝區
 *
 * rosidl 的 String 是 { char* data; size_t size; size_t capacity; }
 * 反序列化時會檢查 capacity >= 線上字串長度 (含 '\0')，
 * 不夠就整包 deserialize 失敗。
 *
 * 注意：綁定靜態緩衝區之後，絕對不可以再呼叫
 *       robot_interfaces__msg__MechanismCommand__init() / __fini()，
 *       __fini() 會把靜態陣列丟給 allocator.deallocate() → heap 損毀。
 * ------------------------------------------------------------------ */
#define MECH_CMD_NAME_CAPACITY   64
#define MECH_CMD_JSON_CAPACITY  256

static char mech_cmd_name_buf[MECH_CMD_NAME_CAPACITY];
static char mech_cmd_json_buf[MECH_CMD_JSON_CAPACITY];

static void mechanism_command_msg_bind_buffers(void)
{
  mechanism_command_msg.command_id = 0;

  mech_cmd_name_buf[0] = '\0';
  mechanism_command_msg.command_name.data     = mech_cmd_name_buf;
  mechanism_command_msg.command_name.size     = 0;
  mechanism_command_msg.command_name.capacity = MECH_CMD_NAME_CAPACITY;

  mech_cmd_json_buf[0] = '\0';
  mechanism_command_msg.arg_json.data     = mech_cmd_json_buf;
  mechanism_command_msg.arg_json.size     = 0;
  mechanism_command_msg.arg_json.capacity = MECH_CMD_JSON_CAPACITY;
}

void uros_init(void) {
  // Initialize micro-ROS
  rmw_uros_set_custom_transport(
    true,
    (void *) &huart3,
    cubemx_transport_open,
    cubemx_transport_close,
    cubemx_transport_write,
    cubemx_transport_read);

  rcl_allocator_t freeRTOS_allocator = rcutils_get_zero_initialized_allocator();

  freeRTOS_allocator.allocate = microros_allocate;
  freeRTOS_allocator.deallocate = microros_deallocate;
  freeRTOS_allocator.reallocate = microros_reallocate;
  freeRTOS_allocator.zero_allocate =  microros_zero_allocate;

  (void) rcutils_set_default_allocator(&freeRTOS_allocator);
}

/* 把最後一次錯誤留下來給 debugger 看 */
volatile int uros_last_error = 0;


void uros_agent_status_check(void) {
  switch (status) {
    case AGENT_WAITING:
      handle_state_agent_waiting();
      break;
    case AGENT_AVAILABLE:
      handle_state_agent_available();
      break;
    case AGENT_CONNECTED:
      handle_state_agent_connected();
      break;
    case AGENT_TRYING:
      handle_state_agent_trying();
      break;
    case AGENT_DISCONNECTED:
      handle_state_agent_disconnected();
      break;
    default:
      break;
  }
}

void handle_state_agent_waiting(void) {
  status = (rmw_uros_ping_agent(100, 10) == RMW_RET_OK) ? AGENT_AVAILABLE : AGENT_WAITING;
}

void handle_state_agent_available(void) {
  uros_create_entities();
  status = AGENT_CONNECTED;
}

void handle_state_agent_connected(void) {
  static uint32_t last_ping_tick = 0;
  uint32_t now = osKernelGetTickCount();

  /* 只在週期到了才 ping，其餘時間全部給 executor */
  if ((uint32_t)(now - last_ping_tick) >= AGENT_PING_PERIOD_MS) {
    last_ping_tick = now;
    if (rmw_uros_ping_agent(100, 1) != RMW_RET_OK) {
      ping_fail_count++;
      if (ping_fail_count >= MAX_PING_FAIL_COUNT) {
        status = AGENT_TRYING;
      }
      return;
    }
    ping_fail_count = 0;
  }
  

  rclc_executor_spin_some(&executor, RCL_MS_TO_NS(10));
}

void handle_state_agent_trying(void) {
  if(rmw_uros_ping_agent(50, 10) == RMW_RET_OK){
    status = AGENT_CONNECTED;
    ping_fail_count = 0; // Reset ping fail count
  } else {
    ping_fail_count++;
    if(ping_fail_count >= MAX_PING_FAIL_COUNT){
      status = AGENT_DISCONNECTED;
      ping_fail_count = 0;
    }
  }
}
void handle_state_agent_disconnected(void) {
  uros_destroy_entities();
  status = AGENT_WAITING;
}

void uros_create_entities(void) {
  rcl_ret_t rc;

  allocator = rcl_get_default_allocator();

  init_options = rcl_get_zero_initialized_init_options();
  rcl_init_options_init(&init_options, allocator);
  rcl_init_options_set_domain_id(&init_options, DOMAIN_ID);

  rc = rclc_support_init_with_options(&support, 0, NULL, &init_options, &allocator);
  if (rc != RCL_RET_OK) { uros_last_error = 1000 + (int)rc; }

  rcl_init_options_fini(&init_options);

  rc = rclc_node_init_default(&node, NODE_NAME, "", &support);
  if (rc != RCL_RET_OK) { uros_last_error = 2000 + (int)rc; }

  /* 關鍵：綁定靜態字串緩衝區（取代 MechanismCommand__init） */
  mechanism_command_msg_bind_buffers();

  rc = rclc_subscription_init_default(
    &mechanism_command_sub,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(robot_interfaces, msg, MechanismCommand),
    "/mechanism/command");
  if (rc != RCL_RET_OK) { uros_last_error = 3000 + (int)rc; }

  rc = rclc_executor_init(&executor, &support.context, 1, &allocator);
  if (rc != RCL_RET_OK) { uros_last_error = 4000 + (int)rc; }

  rc = rclc_executor_add_subscription(
    &executor, &mechanism_command_sub, &mechanism_command_msg,
    &mechanism_command_cb, ON_NEW_DATA);
  if (rc != RCL_RET_OK) { uros_last_error = 5000 + (int)rc; }
}

void uros_destroy_entities(void) {
  rmw_context_t* rmw_context = rcl_context_get_rmw_context(&support.context);
  (void) rmw_uros_set_context_entity_destroy_session_timeout(rmw_context, 0);

  // Destroy subscriber
  rcl_subscription_fini(&mechanism_command_sub, &node);

  /* 不要呼叫 MechanismCommand__fini()：字串指向靜態陣列，free 會炸 heap */

  // Destroy node
  rclc_executor_fini(&executor);
  rcl_node_fini(&node);
  rclc_support_fini(&support);
}

void mechanism_command_cb(const void *msgin) {
  const robot_interfaces__msg__MechanismCommand *msg =
      (const robot_interfaces__msg__MechanismCommand *)msgin;
  if (msg == NULL) {
    return;
  }

  mechanism_command_rx_count++;

  /* 除錯用：把 command_name 抄一份出來，Live Expressions 可直接看 */
  size_t n = msg->command_name.size;
  if (n >= sizeof(mechanism_command_name_dbg)) {
    n = sizeof(mechanism_command_name_dbg) - 1;
  }
  memcpy(mechanism_command_name_dbg, msg->command_name.data, n);
  mechanism_command_name_dbg[n] = '\0';

  mechanism_command_id = msg->command_id;
  mechanism_command_pending = true;
}

