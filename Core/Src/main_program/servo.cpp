#include "servo.hpp"
#include <cmath>

#define MAX_SERVOS 10 // 最多支援 10 顆馬達 (可以隨意加大，不耗費太多記憶體)

// 靜態馬達池 (不會動用到危險的 malloc/new)
static Servo* servo_pool[MAX_SERVOS];
static int registered_count = 0;

//initialize the servo state
Servo::Servo() {
    is_active = false;
    current_state = ServoState::IDLE;
    wait_duration = 0;
    wait_start_time = 0;   // 新增：防呆
    current_angle = 0.0f;
    target_angle = 0.0f;   // 新增：防呆
    step_unit = 0.0f;      // 新增：防呆
    pending_time_ms = 0.0f;// 新增：防呆
}
// 註冊馬達，初始化相關參數
void Servo::attach(TIM_HandleTypeDef* timer, uint32_t ch, float per_degree, uint32_t offset) {
    htim = timer;
    channel = ch;
    pulse_per_degree = per_degree;
    pulse_offset = offset;
    current_angle = 0.0f;
    target_angle = 0.0f;
    step_unit = 0.0f;
    is_active = true; // 標記為啟用
    __HAL_TIM_SET_COMPARE(htim, channel, pulse_offset); // 預設歸零

    if (registered_count < MAX_SERVOS) {
        servo_pool[registered_count] = this; // 將自己的指標 (this) 存進陣列
        registered_count++;
    }
}

// 設定目標角度與運作時間，計算每次更新的步進量
void Servo::setTargetAfter(uint32_t delay_ms, float new_target, float time_ms) {
    if (!is_active) return;
    
    // 先把未來的目標存進備用變數
    target_angle = new_target;
    pending_time_ms = time_ms;
    
    wait_duration = delay_ms;
    wait_start_time = osKernelGetTickCount(); 
    
    // 【關鍵修正】進入等待狀態
    current_state = ServoState::WAITING;      
}


// 更新馬達狀態，根據目前狀態進行角度調整
void Servo::update() {
    if (!is_active) return;
    // 狀態機：等待中
    if (current_state == ServoState::WAITING) {
        uint32_t current_time = osKernelGetTickCount();
        
        if ((current_time - wait_start_time) >= wait_duration) {
            
            // 時間到了！直接拿剛才存好的 target_angle 跟「現在真實的」 current_angle 來算數學
            if (pending_time_ms <= 0.0f) {
                current_angle = target_angle;
                step_unit = 0.0f;
            } else {
                float total_steps = pending_time_ms / UPDATE_PERIOD_MS;
                step_unit = (target_angle - current_angle) / total_steps;
            }
            
            current_state = ServoState::RUNNING; // 算好步伐，正式啟動
        }
    }
    if (current_state == ServoState::RUNNING)// 如果正在運作，則更新角度
    {
        if (std::abs(target_angle - current_angle) > std::abs(step_unit)) {
            current_angle += step_unit;
        } else {
            current_angle = target_angle;
            current_state = ServoState::IDLE;
        }
        uint32_t pulse = pulse_offset + static_cast<uint32_t>(current_angle * pulse_per_degree);
        __HAL_TIM_SET_COMPARE(htim, channel, pulse);

    }
}
void Servo::updateAll() {
    for (int i = 0; i < registered_count; i++) {
        if (servo_pool[i] != nullptr) {
            servo_pool[i]->update();
        }
    }
}
