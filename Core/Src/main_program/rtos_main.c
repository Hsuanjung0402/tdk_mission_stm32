/*
 * rtos_main.c
 *
 *  Created on: Jul 15, 2026
 *      Author: hsuanjung
 */



#include "stm32h7xx_hal.h"
#include "main.h"
#include "uros_init.h"
#include "servo_monitor.hpp"
#include "arm_test.hpp"
#include "servo_motor_config.h"
#include "ms_2_monitor.h"
#include "fork.hpp"
#include "cmsis_os2.h"
#include <stdbool.h>

#define shoulder_homing_switch GPIO_PIN_3 
#define elbow_homing_switch GPIO_PIN_4 
#define take_hay_bale 99
#define STRAW_SWITCH_DEBOUNCE_MS 25U

int task_remain = 0, task02 = 0, task03 = 0;
volatile bool limsw = false;
volatile bool Prepared = false;
volatile bool shoulder_lim = false;
volatile bool elbow_lim = false;
volatile bool Homing_arm = false;
volatile bool Homing_fork = false;
volatile int arm_command = 0, fork_command = 0;
volatile bool fork_lim = false;
volatile int Rotate_time = 0; // 1800: can drop
volatile int counter = 0;
volatile bool hay_bale_put = false;

/* 第三關自動夾取（宣告與說明見 uros_init.h） */
volatile uint8_t straw_pick_phase = robot_interfaces__msg__StrawPickStatus__PHASE_IDLE;
volatile uint8_t straw_pick_count = 0;
volatile bool    straw_pick_armed = false;
volatile bool    straw_switch_pressed = false;
volatile bool    straw_pick_request = false;

volatile int target_angle_1 = 248,target_angle_2 = 68;

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim12;
extern TIM_HandleTypeDef htim24;
extern bool hay_bale_took;

void StartDefaultTask(void *argument)
{
	HAL_TIM_Base_Start_IT(&htim2);
	servo_init();
	cpp_arm_init(); 
	cpp_fork_init();
 	uros_init();
	for (;;)
	{
		uros_agent_status_check();
		osDelay(100 / FREQUENCY);
	}
}

void StartTask02(void *argument)
{
	for (;;)
	{
		task02++;
		// /mechanism/command 觸發的機構動作 (command_id 由 mechanism_command_cb 更新)
		switch (mechanism_command_id)
		{
		case 2000: // initialize and set motor mid
			mechanism_command_id = 0;
			MS_2_init();
			break;
		case 201: // pusher extend (both stages)
			mechanism_command_id = 0;
			pusher_extend();
			break;
		case 2101: // pusher extend stage 1
			mechanism_command_id = 0;
			pusher_extend_1();
			break;
		case 2201: // pusher extend stage 2
			mechanism_command_id = 0;
			pusher_extend_2();
			break;
		case 2010: // pusher 升到最高點:
			mechanism_command_id = 0;
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, (uint32_t)(500 + 6.67 * 230));
			break;
		// navigation
		case 202: // pusher extract
			mechanism_command_id = 0;
			pusher_retract();
			break;
		// navigation
		 case 2001:	// 翻回去
			mechanism_command_id = 0;
			MS_2_CCW_down();
			break;
		// navigation
		case 203:	//  servo咬住 box
			mechanism_command_id = 0;
			MS_2_close_blue();
			break;
		case 204:	// 順時針 rotate
			mechanism_command_id = 0;
			MS_2_CW_rotate();
			break;
		case 205:
			mechanism_command_id = 0;
			MS_2_CCW_down();
			break;
		// navigation
		case 206: // servo 放開 box
			mechanism_command_id = 0;
			MS_2_open_blue();
			break;




		// mirror map: plus 20 to each mechanism_command_id
		// navigation
		 case 2021:	// 翻回去
			mechanism_command_id = 0;
			MS_2_CW_down();
			break;
		// navigation
		case 223:	//  servo咬住 box
			mechanism_command_id = 0;
			MS_2_close_pink();
			break;
		case 224:	// counterclock rotate
			mechanism_command_id = 0;
			MS_2_CCW_rotate();
			break;
		case 225:
			mechanism_command_id = 0;
			MS_2_CW_down();
			break;
		// navigation
		case 226: // servo 放開 box
			mechanism_command_id = 0;
			MS_2_open_pink();
			break;



		case 207: // 置中
			mechanism_command_id = 0;
			MS_2_middle();
			break;
		case 2011:	// push 從最高點放平
			mechanism_command_id = 0;
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, (uint32_t)(500 + 6.67 * 142));
			break;



		// Test area Start:
		case 111:
			mechanism_command_id = 0;
			MS_2_CW_time(Rotate_time);
			break;

		case 112:
			mechanism_command_id = 0;
			MS_2_CCW_time(Rotate_time);
			break;

		case 1:// Test Light
			mechanism_command_id = 0;	
			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
			osDelay(1000);
			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
			break;
		
		case 10001:
			mechanism_command_id = 0;
			cpp_arm_homing();
			break;
		case 10002:
			mechanism_command_id = 0;
			cpp_arm_test();
			break;
		case 10003:
			mechanism_command_id = 0;
			cpp_arm_script(arm_command);
			break;
		case 20001:
			mechanism_command_id = 0;
			cpp_fork_homing();
			break;
		case 20003:
			mechanism_command_id = 0;
			cpp_fork_pos(fork_command);
			break;

		case 999:
			mechanism_command_id = 0;
			// init_all();
			break;

		default:
			break;
		}

		// Test area End:



		if (straw_pick_request)
		{
			straw_pick_request = false;
			/* request 之後才 disarm 的話，這裡要再擋一次 */
			if (!straw_pick_armed || straw_pick_phase != robot_interfaces__msg__StrawPickStatus__PHASE_IDLE)
			{
				osDelay(1);
				continue;
			}
			straw_pick_phase = robot_interfaces__msg__StrawPickStatus__PHASE_PICKING;
			counter++;
			cpp_arm_script(take_hay_bale);
			while(!hay_bale_took){
				osDelay(1);
			}
			cpp_arm_script(counter);
			while(!hay_bale_put){
				if( counter >= 4 ){
					cpp_fork_pos(2);
				}
			}
			taskENTER_CRITICAL();
			straw_pick_count++;
			straw_pick_phase = robot_interfaces__msg__StrawPickStatus__PHASE_IDLE;
			taskEXIT_CRITICAL();
		}
		osDelay(1);
	}
}

/* PD1 前方極限開關：每 1 ms 在 StartTask03 輪詢，電位穩定 STRAW_SWITCH_DEBOUNCE_MS 才採用。
 * 只在「debounce 後的按下邊緣」且 armed、phase == IDLE 時送出 request，
 * 所以持續壓著不會重複觸發，必須先放開再按才會再觸發。 */
static void straw_switch_poll(void)
{
	static bool raw_last = false;
	static bool stable = false;
	static uint32_t raw_change_tick = 0;

	bool raw = (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_1) == GPIO_PIN_SET);
	uint32_t now = HAL_GetTick();

	if (raw != raw_last)
	{
		raw_last = raw;
		raw_change_tick = now;
	}
	else if (raw != stable && (uint32_t)(now - raw_change_tick) >= STRAW_SWITCH_DEBOUNCE_MS)
	{
		stable = raw;
		straw_switch_pressed = stable;
		if (stable && straw_pick_armed
			&& straw_pick_phase == robot_interfaces__msg__StrawPickStatus__PHASE_IDLE)
		{
			straw_pick_request = true;
		}
	}
}

void StartTask03(void *argument)
{
		for (;;)
		{
			task03++;
			straw_switch_poll();
			if(!Homing_arm && !Homing_fork)cpp_arm_update();
			task_remain = uxTaskGetStackHighWaterMark(NULL);
			osDelay(1);
		}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (HAL_GPIO_ReadPin(GPIOB, GPIO_Pin) == GPIO_PIN_SET)
	{
		limsw = true;
	}

	if (HAL_GPIO_ReadPin(GPIOD, GPIO_Pin) == GPIO_PIN_SET)
	{
		/* PD1 (Trigger) 改由 StartTask03 輪詢 + debounce，這裡不再處理 */
		if (GPIO_Pin == GPIO_PIN_3)
		{
			fork_lim = true;
		}
	}
	// if (HAL_GPIO_ReadPin(GPIOD, GPIO_Pin) == GPIO_PIN_SET)
	// {
	// 	if (GPIO_Pin == GPIO_PIN_1)
	// 	{
	// 		uint32_t trigger_current_time = HAL_GetTick();
	// 		uint32_t trigger_last_time;
	// 		if (trigger_current_time - trigger_last_time >= 2000)
	// 		{
	// 			trigger++;
	// 		}
	// 		trigger_last_time = trigger_current_time;
	// 	}
	// }

	// uint32_t trigger_current_time = HAL_GetTick();
	// if (GPIO_Pin == GPIO_PIN_1)
	// {
	// 	if (HAL_GPIO_ReadPin(GPIOD, GPIO_Pin) == GPIO_PIN_SET)
	// 	{
	// 		uint32_t trigger_last_time;
	// 		if (trigger_current_time - trigger_last_time >= 2000)
	// 		{
	// 			trigger++;
	// 		}
	// 		trigger_last_time = trigger_current_time;
	// 	}
	// }
}
