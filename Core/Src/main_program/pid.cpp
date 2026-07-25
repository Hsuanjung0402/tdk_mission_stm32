#include "pid.hpp"

PID::PID() {
    Kp = 0.0f; Ki = 0.0f; Kd = 0.0f;
    error_sum = 0.0f; last_error = 0.0f; output_limit = 0.0f;
    
}

PID::PID(float p, float i, float d, float limit) {
    Kp = p; Ki = i; Kd = d;
    output_limit = limit;
    error_sum = 0.0f; last_error = 0.0f;
}

void PID::setParameters(float p, float i, float d, float limit) {
    Kp = p; Ki = i; Kd = d;
    output_limit = limit;
    
    reset();
}

float PID::compute(float target, float current, float dt) {
    if (dt <= 0.0f) return 0.0f;

    float error = target - current;
    error_sum += error * dt;

    // 積分防風飽和 (避免誤差無限疊加)
    if (error_sum > output_limit) error_sum = output_limit;
    if (error_sum < -output_limit) error_sum = -output_limit;

    float d_error = (error - last_error) / dt;
    last_error = error;

    float output = (Kp * error) + (Ki * error_sum) + (Kd * d_error);

    // 限制最終輸出不超過 PWM 上限
    if (output > output_limit) output = output_limit;
    if (output < -output_limit) output = -output_limit;

    return output;
}

void PID::reset() {
    error_sum = 0.0f;
    last_error = 0.0f;
}