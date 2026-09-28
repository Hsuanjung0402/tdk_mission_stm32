/*
 * servo_monitor.cpp
 *
 *  Created on: Jul 24, 2026
 *      Author: hsuanjung
 */


#include "servo_oop.hpp"
#include "servo_monitor.hpp"
#include "servo_motor_config.h"
#include "dc_motor_config.h"
#include "dc_driver_oop.hpp"
#include "ms_2.hpp"
#include "cmsis_os2.h"

extern TIM_HandleTypeDef htim3;
extern volatile bool limsw;
extern dc_driver screen;

volatile uint8_t mission_delay_time = 100;
volatile int target_speed = 400;

//                pwm      channel     prepare_angle   initial_angle   target_angle   period   per   min_pwm   max_pwm
servo servo_2_1(&htim3, TIM_CHANNEL_1,     248,             248,           170,        1000,    6.67,   500,     2500);
servo servo_2_2(&htim3, TIM_CHANNEL_2,     68,              68,            150,        1000,    6.67,   500,     2500);

void servo_init(){
    servo_2_1.initial_servo();
    servo_2_2.initial_servo();
}

void pusher_extend_1(){
    servo::set_angle_sync(servo_2_1, servo_2_2, 1);
}

void pusher_extend_2(){
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, ( 500 + 6.67 * 87 ));
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, ( 500 + 6.67 * 142 ));
}

void pusher_extend(){
    pusher_extend_1();
    osDelay(100);
    pusher_extend_2();
}

void pusher_retract(){
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, ( 500 + 6.67 * 170 ));
    osDelay(100);
    servo::set_angle_sync(servo_2_1, servo_2_2, 0);
}
