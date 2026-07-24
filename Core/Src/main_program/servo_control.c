/*
 * servo_control.c
 *
 *  Created on: Jul 16, 2026
 *      Author: hsuanjung
 */


#include "servo_control.h"
#include "cmsis_os2.h"
#include "stm32h7xx_hal.h"
#include "servo_motor_config.h"

extern TIM_HandleTypeDef htim3;

volatile int period = 20;
float pulse, unit;

// void servo_init()
// {
// 	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 500 + angle_1_1 * per_1);
// 	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 500 + angle_2_1 * per_2);
// }

// void set_servo_angle(int num , float target_angle , float last_angle ){
// 	float delta = target_angle - last_angle;
// 	unit = delta / period;
// 	target_angle = last_angle + unit;
// 	for( int i = 0 ; i < period ; i ++ ){
// 		if( num == 1 )pulse = 500 + (target_angle * per_1 );
// 		if( num == 2 )pulse = 500 + (target_angle * per_2 );

// 		if( num == 1 )__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, pulse);
// 		if( num == 2 )__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, pulse);
// 		osDelay(1);
// 		target_angle += unit;
// 	}
// }
