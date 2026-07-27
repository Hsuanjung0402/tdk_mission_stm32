#include "cmsis_os2.h"
#include "stm32h7xx_hal.h"
#include "servo.hpp"
#include "arm_test.hpp"
#include "encoder_dc.hpp"
#include "pid.hpp"
#include "limit_sw.hpp"
extern TIM_HandleTypeDef htim4, htim23, htim12; 
Servo servo_base, servo_rotate, servo_claw, servo_wrist;
Encoder elbow(5.0f, 0.0f, 0.0f, 3199.0f),
        shoulder(5.0f, 0.0f, 0.0f, 3199.0f);
LimitSwitch elbow_homing_switch(GPIOG, GPIO_PIN_2); // 歸零用的微動開關
LimitSwitch shoulder_homing_switch(GPIOG, GPIO_PIN_3); // 歸零用的微動開關

volatile bool is_homing_done[2] = {false, false}; // [0] for elbow, [1] for shoulder

float target_rpm = 3.0f;
// 【調整 1】加上 static，讓這些 ID 變成這份檔案的專屬變數，避免跟其他檔案變數撞名

int arm_init(void)
{
	  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 1500);
	  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, 1500);
	  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 1500);
	  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, 1500);

	  //tim23 encoder
	  HAL_TIM_Encoder_Start(&htim23, TIM_CHANNEL_ALL);
	  // 強制啟動 TIM3 和 TIM4 的 PWM 輸出
	  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
	  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
	  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
	  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);

	  HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1);
      
    
    servo_base.attach(&htim4, TIM_CHANNEL_1, 6.6667f, 1500);
    servo_rotate.attach(&htim4, TIM_CHANNEL_2, 6.6667f, 1500);
    servo_claw.attach(&htim4, TIM_CHANNEL_3, 6.6667f, 1500);
    servo_wrist.attach(&htim4, TIM_CHANNEL_4, 6.6667f, 1500);
    elbow.attach(&htim23, 26400.0f, &htim12, TIM_CHANNEL_1, GPIOD, GPIO_PIN_11);
    shoulder.attach(&htim23, 26400.0f, &htim12, TIM_CHANNEL_2, GPIOD, GPIO_PIN_10);
    

    return 0;
}
void homing()
{
    is_homing_done[0] = false; //reset elbow homing status
    is_homing_done[1] = false; //reset shoulder homing status

    elbow.setTargetAngleAfter(0,300.0f,2000);
    shoulder.setTargetAngleAfter(0,300.0f,2000);
    // 持續檢查，直到開關被穩穩按下
    while (!shoulder_homing_switch.isPressed()) {
        osDelay(5); 
    }
    shoulder.reset();
    is_homing_done[1] = true;


    while (!elbow_homing_switch.isPressed()) {
        osDelay(5); 
    }

    // 撞到了！立刻重置馬達計數，這裡就是絕對 0 度
    elbow.reset();
    is_homing_done[0] = true;



}
int arm_test(void)
{
    homing();
    osDelay(1000);

    servo_base.setTargetAfter(1000, 10.0f, 2000.0f);
    servo_rotate.setTargetAfter(1200, 7.0f, 1000.0f);

    //servo_rotate.setTargetAfter(2000, 120.0f, 1000.0f);
    //servo_claw.setTargetAfter(8000, 30.0f, 1000.0f);

    shoulder.reset();
    elbow.setTargetAngleAfter(5000, 90.0f, 2000);




    return 0;
}

extern "C" 
{
    // 實作給 C 語言呼叫的橋接函式
    void cpp_arm_init(void) {
        arm_init(); // 呼叫你寫好的初始化
    }

    void cpp_arm_test(void) {
        arm_test(); // 呼叫你寫好的測試劇本
    }

    void cpp_arm_update(void) {
        Servo::updateAll(); // 呼叫最核心的靜態更新函式
        Encoder::updateAll(0.01f); // 假設每次更新間隔 0.01 秒 (10 ms)

    }

    void arm_exti_handler(uint16_t GPIO_Pin)
    {
        // 這裡才是真正的判斷邏輯！
        if (GPIO_Pin == elbow_homing_switch.getPin())
        {
            if (elbow_homing_switch.checkInterrupt())
            {
                elbow.reset();          // 瞬間煞停！
                is_homing_done[0] = true;  // 通知任務
            }
        }
        else if (GPIO_Pin == shoulder_homing_switch.getPin())
        {
            if (shoulder_homing_switch.checkInterrupt())
            {
                shoulder.reset();       // 瞬間煞停！
                is_homing_done[1] = true;  // 通知任務
            }
        }
    }
}

