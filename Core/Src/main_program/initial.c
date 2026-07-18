/*
 * initial.c
 *
 *  Created on: Jul 16, 2026
 *      Author: hsuanjung
 */

#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"
#include "initial.h"

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

void init_timer(){
    HAL_TIM_Base_Start_IT(&htim2);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_ALL );
}

