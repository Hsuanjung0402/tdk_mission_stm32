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


    // 👉 關鍵：把自己的指標 (this) 放進靜態池子裡
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
    
    int32_t delta = current_count - last_count;
    current_rpm = (static_cast<float>(delta) / ppr) * (60.0f / dt_seconds);
    last_count = current_count; 

    // 計算 PID 輸出動力
    float power = pid_controller.compute(target_rpm, current_rpm, dt_seconds);

    // MD31C 輸出控制：DIR 決定方向，PWM 決定絕對大小 (必須是正數)
    if (power > 0) {
        HAL_GPIO_WritePin(dir_port, dir_pin1, GPIO_PIN_SET); // 正轉方向
        __HAL_TIM_SET_COMPARE(pwm_htim, pwm_channel, static_cast<uint32_t>(power));
    } else if (power < 0) {
        HAL_GPIO_WritePin(dir_port, dir_pin1, GPIO_PIN_RESET); // 反轉方向
        __HAL_TIM_SET_COMPARE(pwm_htim, pwm_channel, static_cast<uint32_t>(-power)); // 取絕對值轉成正 PWM
    } else {
        __HAL_TIM_SET_COMPARE(pwm_htim, pwm_channel, 0); // 停止
    }
}

void Encoder::reset() {
    if (!is_active) return;
    __HAL_TIM_SET_COUNTER(enc_htim, 0);
    current_count = 0;
    last_count = 0;
    current_rpm = 0.0f;
    target_rpm = 0.0f;
    pid_controller.reset();
    __HAL_TIM_SET_COMPARE(pwm_htim, pwm_channel, 0); // 停止 PWM 輸出
}

// 👉 靜態方法：掃描陣列，更新所有已註冊的編碼器
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