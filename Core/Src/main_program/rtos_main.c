/*
 * rtos_main.c
 *
 *  Created on: Jul 15, 2026
 *      Author: hsuanjung
 */



#include "stm32h7xx_hal.h"
#include "mission_level.hpp"
#include "uros_init.h"
#include "cmsis_os2.h"

int tim2 = 0;

extern TIM_HandleTypeDef htim2;

void StartDefaultTask(void *argument)
{
	HAL_TIM_Base_Start_IT(&htim2);
	uros_init();
	for (;;)
	{
		uros_agent_status_check();
		osDelay(100/FREQUENCY);
		osDelay(1);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */
  if (htim->Instance == TIM2)
  {
	tim2++;
    mission_control();
  }
  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM17)
  {
    HAL_IncTick();
  }
}
