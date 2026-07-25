#ifndef ENCODER_HPP
#define ENCODER_HPP
#include "stm32h7xx_hal.h"
#include "pid.hpp"

#define MAX_ENCODERS 4 // 最多支援 4 顆直交編碼器



// --- 類別宣告 ---
class Encoder {
public:
    TIM_HandleTypeDef* enc_htim;
    //hardware
    TIM_HandleTypeDef*  pwm_htim; // 指向硬體計時器的指標
    uint32_t pwm_channel;        // PWM 通道號碼 (TIM_CHANNEL_1, TIM_CHANNEL_2, ...)
    GPIO_TypeDef* dir_port;          // PWM 對應的 GPIO 設定
    uint16_t dir_pin1;           // PWM 對應的 GPIO 腳位
    uint16_t dir_pin2;           // PWM 對應的 GPIO 腳位

             


    //encoder status
    float target_rpm;   
    float ppr;             // 一圈的脈衝數 (Pulses Per Revolution)
    int32_t current_count; // 當前累積脈衝數
    int32_t last_count;    // 上次更新時的脈衝數
    float current_rpm;     // 當前轉速
    bool is_active;
    PID pid_controller;
    Encoder(float kp, float ki, float kd, float max_pwm); 

    void attach(TIM_HandleTypeDef* timer, float pulses_per_rev, 
        TIM_HandleTypeDef* motor_timer, uint32_t motor_channel,GPIO_TypeDef* port, uint16_t pin1);
    void update(float dt_seconds);
    void reset();
    void setTargetRPM(float rpm);

    // 👉 像你的 Servo 一樣，提供一個靜態方法來一次更新所有註冊的編碼器
    static void updateAll(float dt_seconds); 
};

// --- 對外 C API 宣告 ---
// (為了讓 main.c 或純 C 的 FreeRTOS 任務能呼叫，我們還是留一個簡單的橋樑)
#ifdef __cplusplus
extern "C" {
#endif

void Encoder_Update_All_C(float dt_seconds);

#ifdef __cplusplus
}
#endif

#endif // ENCODER_HPP