#include <cmath>
#include "stm32h7xx_hal.h"
#include "cmsis_os2.h"

const float L1 = 12.0f; // 第一節 (第一個手肘到第二個手肘)
const float L2 = 10.0f; // 第二節 (第二個手肘到手腕)
const float L3 = 8.0f;  // 第三節 (手腕到夾爪尖端)

// 用一個結構體來打包算出來的三個角度
struct ArmAngles {
    float theta1; // 第一個手肘目標角度
    float theta2; // 第二個手肘目標角度
    float theta3; // 手腕目標角度
    bool reachable; // 目標是否太遠抓不到？
};

/**
 * 算出讓夾爪抵達 (target_X, target_Y) 且保持角度 phi 的馬達角度
 * @param phi_deg: 夾爪的終極姿態，-90度代表垂直朝下，0度代表水平
 */
ArmAngles calculateIK(float target_X, float target_Y, float phi_deg = -90.0f) {
    ArmAngles result;
    result.reachable = false; // 預設為失敗

    // 把角度轉成 C++ math 用的弧度 (Radian)
    float phi_rad = phi_deg * (M_PI / 180.0f);

    // 步驟 1：反推手腕的座標 (Xw, Yw)
    float Xw = target_X - L3 * std::cos(phi_rad);
    float Yw = target_Y - L3 * std::sin(phi_rad);

    // 步驟 2：算出手腕到原點的直線距離 D
    float D_sq = Xw * Xw + Yw * Yw;
    float D = std::sqrt(D_sq);

    // 【防呆機制】：如果手腕的位置超出手臂極限，就直接回傳失敗
    if (D > (L1 + L2) || D < std::abs(L1 - L2)) {
        return result;
    }

    // 步驟 3：算第二個手肘 (theta2) - 餘弦定理求內角 alpha
    float cos_alpha = (L1 * L1 + L2 * L2 - D_sq) / (2.0f * L1 * L2);
    float alpha_rad = std::acos(cos_alpha);
    // 假設手肘打直是 0 度，往內折是正角度 (外角)
    float theta2_rad = M_PI - alpha_rad;

    // 步驟 4：算第一個手肘 (theta1)
    float gamma = std::atan2(Yw, Xw);
    float beta = std::atan2(L2 * std::sin(theta2_rad), L1 + L2 * std::cos(theta2_rad));
    // 這裡我們選擇讓手肘「往上凸」的姿態 (通常機械手臂的預設姿態)
    float theta1_rad = gamma + beta;

    // 步驟 5：算手腕 (theta3) 補償角度
    // 讓三個關節加起來剛好等於我們要求的 phi_rad
    float theta3_rad = phi_rad - theta1_rad - theta2_rad;

    // 步驟 6：把弧度轉回日常使用的度數，存進結果裡
    result.theta1 = theta1_rad * (180.0f / M_PI);
    result.theta2 = theta2_rad * (180.0f / M_PI);
    result.theta3 = theta3_rad * (180.0f / M_PI);
    result.reachable = true; // 計算成功！

    return result;
}
void move_straight_down() {
    float start_y = 40.0f;
    float end_y = 30.0f;
    float fixed_x = 40.0f;   // X 軸固定在 40
    float phi = -90.0f;      // 夾爪永遠保持垂直朝下
    
    int steps = 20;                  // 把這段距離切成 20 份 (步數越多，直線越平滑)
    uint32_t time_per_step = 50;     // 每走一步給 50 毫秒 (20步 * 50ms = 1000ms = 1秒)
    
    // 算出每一次迴圈 Y 要改變多少 ( (30 - 40) / 20 = -0.5 )
    float y_step_size = (end_y - start_y) / steps; 

    // 開始直線移動迴圈
    for (int i = 1; i <= steps; i++) {
        // 算出當下這一步的 Y 座標
        float current_y = start_y + (i * y_step_size);
        
        // 丟進 IK 大腦算角度
        ArmAngles angles = calculateIK(fixed_x, current_y, phi);
        
        if (angles.reachable) {
            // 立刻更新馬達目標，要求在 time_per_step (50毫秒) 內轉到
            // 注意：這裡 delay 設為 0，因為我們用 FreeRTOS 的 osDelay 來控制節奏
            //servo_base.setTargetAfter(0, angles.theta1, time_per_step);
            //servo_rotate.setTargetAfter(0, angles.theta2, time_per_step);
            //servo_wrist.setTargetAfter(0, angles.theta3, time_per_step);
        } else {
            // 如果算出來發現手太短摸不到，就立刻跳出迴圈保護硬體
            break; 
        }
        
        // 【關鍵！】讓 FreeRTOS 任務停下來等待這 50 毫秒走完，再餵下一個微小點位
        osDelay(time_per_step); 
    }
}
