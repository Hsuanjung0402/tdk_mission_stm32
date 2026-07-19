/*
 * rtos_main.c
 *
 *  Created on: Jul 15, 2026
 *      Author: hsuanjung
 */



#include "stm32h7xx_hal.h"
#include "uros_init.h"
#include "initial.h"
#include "servo_control.h"
#include "motor_config.h"
#include "cmsis_os2.h"
#include <stdbool.h>

int task_remain = 0;
volatile int mission = 0, angle = 0;
bool limsw = false;
int task02 = 0;

void StartDefaultTask(void *argument)
{
	init_timer();
	servo_init();
	uros_init();
	for (;;)
	{
		uros_agent_status_check();
		osDelay(100/FREQUENCY);
		osDelay(1);
	}
}

void StartTask02(void *argument)
{
	for (;;)
	{
		task02++;
		switch (mission)
		{
		case 1:
			mission = 0;
			limsw = false;
			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
			osDelay(1000);
			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
			break;

		case 2:
			mission = 0;
			set_servo_angle(2, angle_2_2, angle_2_1);
			osDelay(500);
			set_servo_angle(1, angle_1_2, angle_1_1);
			osDelay(1000);
			set_servo_angle(1, angle_1_1, angle_1_2);
			osDelay(500);
			set_servo_angle(2, angle_2_1, angle_2_2);
		default:
			break;
		}
		task_remain = uxTaskGetStackHighWaterMark(NULL);
		osDelay(1);
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	 if (HAL_GPIO_ReadPin(GPIOB, GPIO_Pin) == GPIO_PIN_SET) {
		mission = 1;
		limsw = true;
	 }
}
