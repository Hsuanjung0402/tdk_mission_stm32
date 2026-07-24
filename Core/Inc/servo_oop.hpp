/*
 * servo_oop.hpp
 *
 *  Created on: Jul 24, 2026
 *      Author: hsuanjung
 */

#ifndef INC_SERVO_OOP_HPP_
#define INC_SERVO_OOP_HPP_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx_hal.h"
#include "stm32h723xx.h"

class servo
{
public:
    servo(TIM_HandleTypeDef* pwm, uint16_t channel, int prepare_angle, int initial_angle, int target_angle, int period, float per, int min_pwm, int max_pwm):
    _pwm(pwm), _channel(channel), _prepare_angle(prepare_angle), _initial_angle(initial_angle), _target_angle(target_angle), _period(period), _per(per) , _min_pwm(min_pwm), _max_pwm(max_pwm){}
    ~servo() = default;

    void initial_servo();
    void set_angle(bool dir);

private:
    TIM_HandleTypeDef* _pwm;
    uint16_t _channel;
    int _prepare_angle;
    int _initial_angle;
    int _target_angle;
    int _current_angle;
    int _period;
    int _min_pwm;
    int _max_pwm;
    int _pulse;
    float _per;
    float _unit;
    float _delta;
};


#ifdef __cplusplus
}
#endif



#endif /* INC_SERVO_OOP_HPP_ */
