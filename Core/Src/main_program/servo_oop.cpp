/*
 * servo_oop.cpp
 *
 *  Created on: Jul 24, 2026
 *      Author: hsuanjung
 */

#include "servo_oop.hpp"
#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"
#include "cmsis_os2.h"

void servo::initial_servo(){
    HAL_TIM_PWM_Start( _pwm, _channel);
    __HAL_TIM_SET_COMPARE( _pwm, _channel, _min_pwm + _per * _prepare_angle);
}

void servo::set_angle(bool dir){
    if(dir){
        _delta = _target_angle - _initial_angle;
        _unit = _delta / _period;
        _current_angle = _initial_angle + _unit;
        for( int i = 0 ; i < _period ; i ++ ){
            _pulse = _min_pwm + ( _current_angle * _per );
            if( _pulse > _max_pwm ) _pulse = _max_pwm;
            __HAL_TIM_SET_COMPARE( _pwm, _channel, _pulse);
            osDelay(1);
            _current_angle += _unit;
        }   
    }else{
        _delta = _initial_angle - _target_angle ;
        _unit = _delta / _period;
        _current_angle = _target_angle + _unit;
        for( int i = 0 ; i < _period ; i ++ ){
            _pulse = _min_pwm + ( _current_angle * _per );
            if( _pulse > _max_pwm ) _pulse = _max_pwm;
            __HAL_TIM_SET_COMPARE( _pwm, _channel, _pulse);
            osDelay(1);
            _current_angle += _unit;
        }
    }
}

