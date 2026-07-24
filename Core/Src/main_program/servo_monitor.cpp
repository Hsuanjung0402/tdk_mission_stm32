/*
 * servo_monitor.cpp
 *
 *  Created on: Jul 24, 2026
 *      Author: hsuanjung
 */


#include "servo_oop.hpp"
#include "servo_monitor.hpp"
#include "servo_motor_config.h"
#include "cmsis_os2.h"

extern TIM_HandleTypeDef htim3;

servo servo_2_1(&htim3, TIM_CHANNEL_1, 0 ,  0, 180, 300, 7.3 , 500, 2500);
servo servo_2_2(&htim3, TIM_CHANNEL_2, 42, 42, 126, 300, 6.65, 500, 2500);
servo servo_3_1(&htim3, TIM_CHANNEL_3, 47, 47, 128, 300, 7.3 , 500, 2500);

void servo_init(){
    servo_2_1.initial_servo();
    servo_2_2.initial_servo();
    servo_3_1.initial_servo();
}

void pusher(){
    servo_2_2.set_angle(1);
    osDelay(500);
    servo_2_1.set_angle(1);
    osDelay(1000);
    servo_2_1.set_angle(0);
    osDelay(500);
    servo_2_2.set_angle(0);
}
