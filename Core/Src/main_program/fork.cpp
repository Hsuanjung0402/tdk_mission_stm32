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
Encoder fork_motor( 3.0f, 0.3f, 0.0f, 10000.0f);
extern bool fork_lim;
extern bool Homing_fork;

extern TIM_HandleTypeDef htim4, htim8, htim23;

void fork_init(){
    fork_motor.attach(&htim8, 22000.0f, &htim23, TIM_CHANNEL_4, GPIOG, GPIO_PIN_3);
}

void fork_homing(){
    Homing_fork = true;
    fork_motor.homing_cw(3000);
    osDelay(1000);
    fork_lim = false;
    fork_motor.homing_ccw(3000);
    while(!fork_lim){
        osDelay(1);
    }
    fork_motor.homing_cw(200);
    osDelay(200);
    fork_motor.reset();
    Homing_fork = false;
}

void fork_pos(int Fork_command){
    // MAX SPEED: 36 DEGREES / 1000 M SEC = .0036
    switch (Fork_command)
    {
    case 1: // For level 1
        fork_motor.setTargetAngleAfter(0, 70, 1000);
        break;
    case 2: // For level 2
        fork_motor.setTargetAngleAfter(0, 140, 7000);
        break;
    case 99:
        fork_motor.setTargetAngleAfter(0, test_angle, 5000);
    default:
        break;
    }
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