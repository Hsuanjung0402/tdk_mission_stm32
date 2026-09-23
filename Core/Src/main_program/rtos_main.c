/*
 * rtos_main.c
 *
 *  Created on: Jul 15, 2026
 *      Author: hsuanjung
 */



#include "stm32h7xx_hal.h"
#include "uros_init.h"
#include "servo_monitor.hpp"
#include "arm_test.hpp"
#include "servo_motor_config.h"
#include "ms_2_monitor.h"
#include "cmsis_os2.h"
#include <stdbool.h>

#define shoulder_homing_switch GPIO_PIN_3 
#define elbow_homing_switch GPIO_PIN_4 

int task_remain = 0, task02 = 0;
volatile bool limsw = false;
volatile bool Prepared = false;
volatile bool shoulder_lim = false;
volatile bool elbow_lim = false;
volatile bool Homing = false;
volatile int arm_command = 0;

volatile int target_angle_1 = 248,target_angle_2 = 68;

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim12;
extern TIM_HandleTypeDef htim24;

void StartDefaultTask(void *argument)
{
	HAL_TIM_Base_Start_IT(&htim2);
	servo_init();
	// cpp_arm_init(); 
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
		case 207: // 置中
			mechanism_command_id = 0;
			MS_2_middle();
			break;
		case 2011:	// push 從最高點放平
			mechanism_command_id = 0;
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, (uint32_t)(500 + 6.67 * 142));
			break;





		case 999:
			mechanism_command_id = 0;
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, (uint32_t)(500 + 6.67 * target_angle_1));
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, (uint32_t)(500 + 6.67 * target_angle_2));
			break;

		case 1:// Test Light
			mechanism_command_id = 0;	
			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
			osDelay(1000);
			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
			break;
		case 6:
			mechanism_command_id = 0;
			MS_2_CCW_rotate();
			break;
		case 8:
			mechanism_command_id = 0;
			MS_2_close_pink();
			break;
		
		case 10:
			mechanism_command_id = 0;
			MS_2_open_pink();
			break;
		
		case 16:
			mechanism_command_id = 0;
			HAL_TIM_PWM_Start( &htim12, TIM_CHANNEL_2);
			HAL_TIM_Encoder_Start(&htim24, TIM_CHANNEL_ALL);
    		__HAL_TIM_SET_COUNTER(&htim24, 0); 
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET); 
			__HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_2, (uint32_t)(50));
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





		default:
			break;
		}
	osDelay(1);
	}
}

void StartTask03(void *argument)
{
		for (;;)
		{
			if(!Homing)cpp_arm_update();
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
}
