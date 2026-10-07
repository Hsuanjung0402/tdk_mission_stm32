/*
 * dc_driver_oop.cpp
 *
 *  Created on: Jul 29, 2026
 *      Author: hsuanjung
 */

#include "dc_driver_oop.hpp"
#include "cmsis_os2.h"
#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"

extern volatile bool limsw;

bool dc_driver::init_dc_driver(){
    HAL_TIM_PWM_Start( _pwm, _channel);
    return true;
}

void dc_driver::stop(){
    __HAL_TIM_SET_COMPARE( _pwm, _channel, 0);
}

bool dc_driver::cw(){
    HAL_GPIO_WritePin( _dir_gpio, _dir_pin , GPIO_PIN_RESET);
    _current_pwm = __HAL_TIM_GET_COMPARE(_pwm, _channel);
    _target_pwm = _speed;

    _diff_pwm = _target_pwm - _current_pwm;
    if( _diff_pwm == 0 )return false;

    for( int i = 1 ; i <= _period ; i ++ ){
        _step_pwm = _current_pwm + ( _diff_pwm * i / _period );
        __HAL_TIM_SET_COMPARE( _pwm, _channel, _step_pwm);
        osDelay(1);
    }
    __HAL_TIM_SET_COMPARE( _pwm, _channel, _target_pwm);
    return true;
}

bool dc_driver::ccw(){
    HAL_GPIO_WritePin( _dir_gpio, _dir_pin , GPIO_PIN_SET);
    _current_pwm = __HAL_TIM_GET_COMPARE(_pwm, _channel);
    _target_pwm = _speed;

    _diff_pwm = _target_pwm - _current_pwm;
    if( _diff_pwm == 0 )return false;

    for( int i = 1 ; i <= _period ; i ++ ){
        _step_pwm = _current_pwm + ( _diff_pwm * i / _period );
        __HAL_TIM_SET_COMPARE( _pwm, _channel, _step_pwm);
        osDelay(1);
    }
    __HAL_TIM_SET_COMPARE( _pwm, _channel, _target_pwm);
    return true;
}