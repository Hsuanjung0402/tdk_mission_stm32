/*
 * uros_init.h
 *
 *  Created on: Jul 15, 2026
 *      Author: hsuanjung
 */

#ifndef INC_UROS_UROS_INIT_H_
#define INC_UROS_UROS_INIT_H_

#include "stm32h7xx_hal.h"
#include "cmsis_os.h"

#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <uxr/client/transport.h>
#include <rmw_microxrcedds_c/config.h>
#include <rmw_microros/rmw_microros.h>
#include <std_msgs/msg/int32.h>
#include <std_msgs/msg/bool.h>
#include <robot_interfaces/msg/mechanism_command.h>

#include "uros_config.h"
#include "timers.h"

#ifdef __cplusplus
extern "C" {
#endif

extern rcl_subscription_t                          mechanism_command_sub;
extern robot_interfaces__msg__MechanismCommand      mechanism_command_msg;

/* 由 mechanism_command_cb 更新，供 rtos_main.c 的 StartTask02 讀取後 dispatch */
extern volatile uint16_t  mechanism_command_id;
extern volatile bool      mechanism_command_pending;

/* printf 目前經由 huart3 (與 micro-ROS transport 共用) 無法輸出，
 * 用這個計數器在除錯視窗 (Live Expressions) 確認 callback 是否真的被觸發 */
extern volatile uint32_t  mechanism_command_rx_count;

bool cubemx_transport_open(struct uxrCustomTransport * transport);
bool cubemx_transport_close(struct uxrCustomTransport * transport);
size_t cubemx_transport_write(struct uxrCustomTransport* transport, const uint8_t * buf, size_t len, uint8_t * err);
size_t cubemx_transport_read(struct uxrCustomTransport* transport, uint8_t* buf, size_t len, int timeout, uint8_t* err);

void * microros_allocate(size_t size, void * state);
void microros_deallocate(void * pointer, void * state);
void * microros_reallocate(void * pointer, size_t size, void * state);
void * microros_zero_allocate(size_t number_of_elements, size_t size_of_element, void * state);

typedef enum {
    AGENT_WAITING,
    AGENT_AVAILABLE,
    AGENT_CONNECTED,
    AGENT_TRYING,
    AGENT_DISCONNECTED
} agent_status_t;

void uros_init(void);

void uros_agent_status_check(void);

void handle_state_agent_waiting(void);
void handle_state_agent_available(void);
void handle_state_agent_connected(void);
void handle_state_agent_trying(void);
void handle_state_agent_disconnected(void);

void uros_create_entities(void);
void uros_destroy_entities(void);

void mechanism_command_cb(const void* msgin);

#ifdef __cplusplus
}
#endif



#endif /* INC_UROS_UROS_INIT_H_ */
