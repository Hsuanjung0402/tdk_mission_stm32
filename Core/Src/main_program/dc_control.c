/*
 * dc_control.c
 *
 *  Created on: Jul 20, 2026
 *      Author: hsuanjung
 */

#include "dc_control.h"
#include "cmsis_os2.h"
#include "stm32h7xx_hal.h"

void dc_motor(int dir){
    switch (dir)
    {
    case 0:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4,GPIO_PIN_RESET);
        break;
    case 1:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4,GPIO_PIN_RESET);
        break;
    case -1:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4,GPIO_PIN_SET);
        break;
    default:
        break;
    }
}