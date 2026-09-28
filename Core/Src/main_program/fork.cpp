/*
 * fork.cpp
 *
 *  Created on: Sep 26, 2026
 *      Author: hsuanjung
 */

#include "cmsis_os2.h"
#include "stm32h7xx.h"
#include "encoder_dc.hpp"
volatile float test_kp = 2.0f;
volatile int test_angle = 30;
Encoder fork_motor( 3.0f, 0.1f, 0.0f, 10000.0f);
extern bool fork_lim;
extern bool Homing_fork;

extern TIM_HandleTypeDef htim4, htim8;

void fork_init(){
    fork_motor.attach(&htim8, 22000.0f, &htim4, TIM_CHANNEL_2, GPIOD, GPIO_PIN_15);
}

void fork_homing(){
    Homing_fork = true;
    fork_motor.homing_cw(4000);
    osDelay(2500);
    fork_lim = false;
    fork_motor.homing_ccw(6000);
    while(!fork_lim){
        osDelay(1);
    }
    fork_motor.homing_cw(100);
    osDelay(200);
    fork_motor.reset();
    Homing_fork = false;
}

void fork_pos(int Fork_command){ // DO NOT ROTATE OVER 260 DEGREES !!!
    // MAX SPEED: 36 DEGREES / 1000 M SEC = .0036
    fork_motor.reset();
    switch (Fork_command)
    {
    case 1: // For level 1
        fork_motor.setTargetAngleAfter(0, 30, 1000);
        break;
    case 2: // For level 2
        fork_motor.setTargetAngleAfter(0, 95, 7000);
        break;
    case 99:
        fork_motor.setTargetAngleAfter(0, test_angle, 5000);
    default:
        break;
    }
    // HAL_TIM_Encoder_Start(&htim8, TIM_CHANNEL_ALL);
    // HAL_GPIO_WritePin( GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
    // HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
    // __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, 5000);
}

extern "C"{
    void cpp_fork_init(){
        fork_init();
    }
    void cpp_fork_homing(){
        fork_homing();
    }
    void cpp_fork_pos(int POS){
        fork_pos(POS);
    }
}