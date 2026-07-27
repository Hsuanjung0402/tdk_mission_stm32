#include "encoder_dc.hpp"
#include "stm32h7xx_hal.h"
#include "pid.hpp"


// 靜態編碼器指標池 (存放物件指標)
static Encoder* encoder_pool[MAX_ENCODERS];
static int registered_count = 0;
// 初始化變數
Encoder::Encoder(float kp, float ki, float kd, float max_pwm)
    :pid_controller(kp, ki, kd, max_pwm) 
{
    current_state = EncoderState::IDLE;
    is_active = false;
    current_count = 0;
    last_count = 0;
    current_rpm = 0.0f;
    ppr = 0.0f;
    enc_htim = nullptr;
}

// 註冊編碼器
void Encoder::attach(TIM_HandleTypeDef* enc_timer, float pulses_per_rev, 
    TIM_HandleTypeDef* motor_timer, uint32_t motor_channel,GPIO_TypeDef* port, uint16_t pin1) {
    enc_htim = enc_timer;
    ppr = pulses_per_rev;

    pwm_htim = motor_timer;
    pwm_channel = motor_channel;
    dir_port = port;
    dir_pin1 = pin1;
    //dir_pin2 = pin2;

    current_count = 0;
    last_count = 0;
    current_rpm = 0.0f;
    target_rpm = 0.0f;
    is_active = true; // 標記為啟用
        
    // 啟動硬體編碼器模式並強制歸零
    HAL_TIM_Encoder_Start(enc_htim, TIM_CHANNEL_ALL);
    __HAL_TIM_SET_COUNTER(enc_htim, 0); 
    
    // 啟動 PWM 輸出並強制歸零
    HAL_TIM_PWM_Start(pwm_htim, pwm_channel); // 啟動 PWM 輸出
    //__HAL_TIM_SET_COUNTER(pwm_htim, 0); // 強制歸零

    if (registered_count < MAX_ENCODERS) {
        encoder_pool[registered_count] = this;
        registered_count++;
    }
}
void Encoder::setTargetRPM(float rpm) {
    target_rpm = rpm;
}

// 單一物件的更新邏輯
void Encoder::update(float dt_seconds) {
    if (!is_active || dt_seconds <= 0.0f) return;
    
    current_count = static_cast<int32_t>(__HAL_TIM_GET_COUNTER(enc_htim));
    uint32_t current_time = HAL_GetTick();

    int32_t delta = current_count - last_count;
    float raw_rpm = (static_cast<float>(delta) / ppr) * (60.0f / dt_seconds);
    last_count = current_count; 
    current_rpm = (0.2f * raw_rpm) + (0.8f * current_rpm); 

    // --- 狀態機邏輯 ---
    if(current_state == EncoderState::WAITING) {
        if ((current_time - wait_start_time) >= stored_delay_ms) {
            current_state = EncoderState::RUNNING;
            
            // 👉 修正 1：絕對關鍵！進入 RUNNING 的瞬間，必須記錄起跑點
            profile_start_time = current_time;
            start_counts = current_count; 
            final_target_counts = static_cast<int32_t>((stored_target_angle / 360.0f) * ppr);

            // 防呆：如果移動時間設為 0，直接瞬移並結束
            if (stored_duration_ms == 0) {
                target_counts = final_target_counts;
                current_state = EncoderState::IDLE;
            }
        }
    }
    else if(current_state == EncoderState::RUNNING) {
        uint32_t elapsed = current_time - profile_start_time; 

        if (elapsed >= stored_duration_ms) {
            target_counts = final_target_counts; 
            current_state = EncoderState::IDLE;  
        }
        else {
            float progress = static_cast<float>(elapsed) / static_cast<float>(stored_duration_ms);
            target_counts = start_counts + static_cast<int32_t>((final_target_counts - start_counts) * progress);
        }
    }


    // --- 輸出控制邏輯 ---
    int32_t error = target_counts - current_count;
    float p_gain = 2.0f; // 比例控制增益
    float power = static_cast<float>(error) * p_gain;

    float max_pwm_limit = 3199.0f; 
    if(power > max_pwm_limit) power = max_pwm_limit;
    else if(power < -max_pwm_limit) power = -max_pwm_limit;

    if (power > 0) {
        HAL_GPIO_WritePin(dir_port, dir_pin1, GPIO_PIN_SET); 
        __HAL_TIM_SET_COMPARE(pwm_htim, pwm_channel, static_cast<uint32_t>(power));
    } else if (power < 0) {
        HAL_GPIO_WritePin(dir_port, dir_pin1, GPIO_PIN_RESET); 
        __HAL_TIM_SET_COMPARE(pwm_htim, pwm_channel, static_cast<uint32_t>(-power)); 
    } else {
        __HAL_TIM_SET_COMPARE(pwm_htim, pwm_channel, 0); 
    }
}
void Encoder::reset() {
    if (!is_active) return;
    __HAL_TIM_SET_COMPARE(pwm_htim, pwm_channel, 0); // 停止 PWM 輸出
    __HAL_TIM_SET_COUNTER(enc_htim, 0);
    final_target_counts = current_count;
    current_count = 0;
    last_count = 0;
    current_rpm = 0.0f;
    target_rpm = 0.0f;
    pid_controller.reset();

}
void Encoder::setTargetAngleAfter(uint32_t delay_ms, float target_angle, uint32_t duration_ms) {
    current_state = EncoderState::WAITING;
    wait_start_time = HAL_GetTick(); // 記錄當前 Tick
    stored_target_angle = target_angle; // 存下目標角度
    stored_duration_ms = duration_ms;
    stored_delay_ms = delay_ms;

    target_counts = static_cast<int32_t>((target_angle / 360.0f) * ppr);
    
}
void Encoder::updateAll(float dt_seconds) {
    for (int i = 0; i < registered_count; i++) {
        if (encoder_pool[i] != nullptr) {
            encoder_pool[i]->update(dt_seconds);
        }
    }
}
// --- 對外 C API 實作 ---
extern "C" {
    // 這是給 main.c 裡面的 StartTask03 呼叫用的
    void Encoder_Update_All_C(float dt_seconds) {
        Encoder::updateAll(dt_seconds);
    }
}
