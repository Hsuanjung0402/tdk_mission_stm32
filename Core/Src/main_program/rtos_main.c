/*
 * rtos_main.c
 *
 *  Created on: Jul 15, 2026
 *      Author: hsuanjung
 */



#include "stm32h7xx_hal.h"

#include "uros_init.h"
#include "cmsis_os.h"

extern TIM_HandleTypeDef htim23;

void StartDefaultTask(void *argument)
{
	HAL_TIM_Base_Start_IT(&htim23);
	uros_init();
	for (;;)
	{
		uros_agent_status_check();
		osDelay(1000/FREQUENCY);
	}
}

