/*
 * ms_2.hpp
 *
 *  Created on: Jul 29, 2026
 *      Author: hsuanjung
 */

#ifndef SRC_MAIN_PROGRAM_MISSION_2_MS_2_HPP_
#define SRC_MAIN_PROGRAM_MISSION_2_MS_2_HPP_

#ifdef __cplusplus
extern "C"{
#endif

#include "dc_driver_oop.hpp"
#include "servo_oop.hpp"

    class ms_2
    {
    public:
        ms_2(GPIO_TypeDef *gpio_210, uint16_t dir_pin_210, TIM_HandleTypeDef *pwm_210, uint32_t channel_210, int period_210, int speed_210, TIM_HandleTypeDef *pwm_202, uint32_t channel_202, int prepare_angle_202, int initial_angle_202, int target_angle_202, float per_202, int min_pwm_202, int max_pwm_202,TIM_HandleTypeDef *pwm_203, uint32_t channel_203, int prepare_angle_203, int initial_angle_203, int target_angle_203, float per_203, int min_pwm_203, int max_pwm_203)
        :motor(pwm_210, channel_210, gpio_210, dir_pin_210, period_210, speed_210), blue(pwm_202, channel_202, prepare_angle_202, initial_angle_202, target_angle_202, per_202, per_202, min_pwm_202, max_pwm_202),pink(pwm_203, channel_203, prepare_angle_203, initial_angle_203, target_angle_203, per_203, per_203, min_pwm_203, max_pwm_203){}
        ~ms_2() = default;

        void init();

        void down_cw();
        void down_ccw();

        void rotate_cw();
        void rotate_ccw();

        void middle();

        void open_pink();
        void open_blue();

        void close_pink();
        void close_blue();

        void cw_time(int);
        void ccw_time(int);
    private:
        dc_driver motor;
        servo pink;
        servo blue;

        enum class pos_ms2{
            middle,
            down_cw,
            down_ccw,
            error
        };

        pos_ms2 POS_ms2;

        bool time_out = false;

        void wait_limit_switch();

    };


#ifdef __cplusplus
}
#endif

#endif /* SRC_MAIN_PROGRAM_MISSION_2_MS_2_HPP_ */
