/*
 * ms_2_monitor.cpp
 *
 *  Created on: Aug 13, 2026
 *      Author: hsuanjung
 */

#include "ms_2.hpp"
#include "main.h"
#include "ms_2_monitor.h"
#include "ms_2_config.h"

extern TIM_HandleTypeDef htim13;
extern TIM_HandleTypeDef htim15;

ms_2 MS_2(GPIOG, GPIO_PIN_4, &htim13, TIM_CHANNEL_1, 10, Speed_210, &htim15, TIM_CHANNEL_2, 90, 90, 175, 7.4, 500, 2500, &htim15, TIM_CHANNEL_1, 191.9, 191.9, 110.1, 7.4, 500, 2500);

extern "C" {

void MS_2_init(){
    MS_2.init();
}

void MS_2_CW_down(){
    MS_2.down_cw();
}

void MS_2_CCW_down(){
    MS_2.down_ccw();
}

void MS_2_CW_rotate(){
    MS_2.rotate_cw();
}

void MS_2_CCW_rotate(){
    MS_2.rotate_ccw();
}

void MS_2_middle(){
    MS_2.middle();
}

void MS_2_open_pink(){
    MS_2.open_pink();
}

void MS_2_open_blue(){
    MS_2.open_blue();
}

void MS_2_close_pink(){
    MS_2.close_pink();
}

void MS_2_close_blue(){
    MS_2.close_blue();
}

void MS_2_CW_time(int TIME){
    MS_2.cw_time(TIME);
}

void MS_2_CCW_time(int TIME){
    MS_2.ccw_time(TIME);
}

}
