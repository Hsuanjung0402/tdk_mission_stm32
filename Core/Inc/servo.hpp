/*
 * servo.hpp
 *
 *  Created on: Jul 24, 2026
 *      Author: jessielin
 */

#ifndef INC_SERVO_HPP_
#define INC_SERVO_HPP_

#ifdef __cplusplus
extern "C" {
#endif
#include "stm32h7xx_hal.h"
#include "cmsis_os2.h"
#ifdef __cplusplus
}
#endif

// ==========================================
// C++ 內部類別 (外部 main.c 看不到，完全封裝)
// ==========================================
#ifdef __cplusplus
enum class ServoState {
    IDLE,       // 閒置中
    WAITING,    // 等待啟動中 (看手錶階段)
    RUNNING     // 運作中
};
class Servo {
private:
    bool is_active; // 標記這個空位有沒有被註冊
    ServoState current_state;
    int pending_target_angle; // 等待啟動的目標角度
    float pending_time_ms; // 等待啟動的運作時間

    TIM_HandleTypeDef* htim;
    uint32_t channel;
    float current_angle, target_angle, step_unit, pulse_per_degree;
    uint32_t pulse_offset;
    static constexpr float UPDATE_PERIOD_MS = 3.0f;
    int wait_duration; // 等待時間 (ms)
    uint32_t wait_start_time; // 等待開始的 Tick 時間  

public:
    Servo(); // 預設建構子
    void attach(TIM_HandleTypeDef* timer, uint32_t ch, float per_degree, uint32_t offset);
    void startAfter(uint32_t delay_ms);
    void setTarget(float new_target, float time_ms);
    void setTargetAfter(uint32_t delay_ms, float new_target, float time_ms);
    void update();
    static void updateAll();
};
#endif

// ==========================================
// C 語言對外介面 (給 main.c 用的 API)
// ==========================================
#ifdef __cplusplus
extern "C" {
#endif

// 註冊一顆馬達，並回傳這顆馬達的 ID (0, 1, 2...)
int Servo_Register(TIM_HandleTypeDef* htim, uint32_t channel, float per_degree, uint32_t offset);

// 用 ID 來控制特定的馬達
void Servo_Set_Target(int servo_id, float target_angle, float time_ms);

// RTOS 背景任務定期呼叫 (會自動更新所有已註冊的馬達)
void Servo_Update_All(void);

#ifdef __cplusplus
}
#endif

#endif // SERVO_HPP
