/*
 * ms_2.cpp
 *
 *  Created on: Jul 29, 2026
 *      Author: hsuanjung
 */

#include "ms_2_config.h"
#include "ms_2.hpp"
#include "dc_driver_oop.hpp"
#include "servo_oop.hpp"
#include "cmsis_os2.h"

#define ratio 4

extern dc_driver screen;
extern servo servo_2_3;
extern volatile bool limsw;

void ms_2::init(){
    pink.initial_servo();   // servo: start PWM, set to initial position
    blue.initial_servo();
    motor.init_dc_driver(); //DC motor: start PWM. DO NOT ROTATE
    down_ccw();
    osDelay(100);
    motor.cw();
    osDelay(init_reverse_time);
    motor.stop();
    POS_ms2 = pos_ms2::middle;
}

void ms_2::down_cw()
{
    if (POS_ms2 == pos_ms2::error)
        return;

    if (POS_ms2 == pos_ms2::down_cw)
        return;

    limsw = false;
    motor.cw();

    wait_limit_switch();
    if (time_out)
    {
        POS_ms2 = pos_ms2::error;
        return;
    }

    motor.stop();
    osDelay(100);
    motor.ccw();
    osDelay(down_reverse_time);
    motor.stop();

    limsw = false;
    POS_ms2 = pos_ms2::down_cw;
}

void ms_2::down_ccw()
{
    if (POS_ms2 == pos_ms2::error)
        return;

    if (POS_ms2 == pos_ms2::down_ccw)
        return;

    limsw = false;
    motor.ccw();

    wait_limit_switch();
    if (time_out)
    {
        POS_ms2 = pos_ms2::error;
        return;
    }

    motor.stop();
    osDelay(100);
    motor.cw();
    osDelay(down_reverse_time);
    motor.stop();

    limsw = false;
    POS_ms2 = pos_ms2::down_ccw;
}

void ms_2::rotate_cw()
{
    if (POS_ms2 == pos_ms2::error)
        return;

    if( POS_ms2 == pos_ms2::down_cw )return;

    motor.cw();
    osDelay(rotate_time);
    motor.stop();
    POS_ms2 = pos_ms2::middle;
}

void ms_2::rotate_ccw()
{
    if (POS_ms2 == pos_ms2::error)
        return;

    if( POS_ms2 == pos_ms2::down_ccw )return;

    motor.ccw();
    osDelay(rotate_time);
    motor.stop();
    POS_ms2 = pos_ms2::middle;
}

void ms_2::middle(){    
    if (POS_ms2 == pos_ms2::error)
        return;
    
    if( POS_ms2 == pos_ms2::down_cw ){
        motor.ccw();
        osDelay(init_reverse_time);
    }
    else if( POS_ms2 == pos_ms2::down_ccw ){
        motor.cw();
        osDelay(init_reverse_time);
    }else{
        return;
    }

    motor.stop();
    POS_ms2 = pos_ms2::middle;
}

void ms_2::wait_limit_switch()
{
    motor.timeout_cnt = 0;
    time_out = false;
    while (!limsw)
    {
        osDelay(1);
        motor.timeout_cnt++;
        if (motor.timeout_cnt >= motor.MAX_TIMEOUT_MS)
        {
            motor.stop();
            time_out = true;
            break;
        }
    }
}

void ms_2::close_blue()
{
    blue.set_angle(1);
}

void ms_2::close_pink()
{
    pink.set_angle(1);
}

void ms_2::open_blue()
{
    blue.set_angle(0);
}

void ms_2::open_pink()
{
    pink.set_angle(0);
}

void ms_2::cw_time(int time){
    if( POS_ms2 == pos_ms2::down_cw )
        return;
    motor.cw();
    osDelay(time);
    motor.stop();
}

void ms_2::ccw_time(int time){
    if( POS_ms2 == pos_ms2::down_ccw )
        return;
    motor.ccw();
    osDelay(time);
    motor.stop();
}