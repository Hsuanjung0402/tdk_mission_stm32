#include "cmsis_os2.h"
#include "stm32h7xx_hal.h"
#include "servo.hpp"
#include "arm_test.hpp"
#include "encoder_dc.hpp"
#include "pid.hpp"
#include "limit_sw.hpp"
extern TIM_HandleTypeDef htim5, htim23, htim12, htim24; 
extern volatile bool Homing;
volatile float test_p = 3.0f;
volatile float test_i = 0.0f;
Servo servo_base, servo_rotate, servo_claw, servo_wrist;
Encoder elbow(3.0f, 0.3f, 0.0f, 3199.0f),
        shoulder(3.0f, 0.0f, 0.0f, 3199.0f);
LimitSwitch elbow_homing_switch(GPIOG, GPIO_PIN_4); // 歸零用的微動開關
LimitSwitch shoulder_homing_switch(GPIOG, GPIO_PIN_3); // 歸零用的微動開關

volatile bool is_homing_done[2] = {false, false}; // [0] for elbow, [1] for shoulder

float target_rpm = 3.0f;
// 【調整 1】加上 static，讓這些 ID 變成這份檔案的專屬變數，避免跟其他檔案變數撞名

int arm_init(void)
{
    HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_4);

    servo_base.attach(&htim5, TIM_CHANNEL_1, 6.6667f, 800);
    servo_rotate.attach(&htim5, TIM_CHANNEL_2, 6.6667f, 800);
    servo_claw.attach(&htim5, TIM_CHANNEL_3, 6.6667f, 900);
    servo_wrist.attach(&htim5, TIM_CHANNEL_4, 7.3f, 600);
    elbow.attach(&htim23, 26400.0f, &htim12, TIM_CHANNEL_1, GPIOD, GPIO_PIN_11);
    shoulder.attach(&htim24, 16100.0f, &htim12, TIM_CHANNEL_2, GPIOD, GPIO_PIN_10);

    arm_init_servo();

    return 0;
}
void homing()
{
    Homing = true;

    is_homing_done[0] = false; //reset elbow homing status
    is_homing_done[1] = false; //reset shoulder homing status

    osDelay(1000);

    shoulder.homing_ccw();
    // // 持續檢查，直到開關被穩穩按下
    while (!shoulder_homing_switch.isPressed()) {
        osDelay(1); 
    }
    shoulder.reset();
    is_homing_done[1] = true;

    elbow.homing_ccw();

    while (!elbow_homing_switch.isPressed()) {
        osDelay(1); 
    }

    // 撞到了！立刻重置馬達計數，這裡就是絕對 0 度
    elbow.reset();
    is_homing_done[0] = true;

    Homing = false;

}

volatile float elbow_test_angle = 8.0f;
volatile float shoulder_test_angle = 25.0f;
volatile float servo_base_test_angle = 35.0f;
volatile float servo_rotate_test_angle = 110.0f;
volatile float servo_claw_test_angle = 45.0f;
volatile float servo_wrist_test_angle = 0.0f;

void arm_homing(void){
    homing();

    osDelay(300);

    elbow.setTargetAngleAfter(0, 8.0f, 500);
    shoulder.setTargetAngleAfter(0, 25.0f, 500);
}

void arm_init_servo(void){
    servo_base.set_current_angle(35.0f);
    servo_wrist.set_current_angle(0.0f);
    servo_rotate.set_current_angle(110.0f);
    servo_claw.set_current_angle(45.0f);
    
    servo_base.setTargetAfter(0, 35.0f, 3000);
    servo_wrist.setTargetAfter(0, 0.0f, 3000);
    servo_rotate.setTargetAfter(0, 110.0f, 3000);
    servo_claw.setTargetAfter(0, 45.0f, 3000);
}

int arm_test(void)
{
    elbow.setTargetAngleAfter(0, elbow_test_angle, 500);
    shoulder.setTargetAngleAfter(0, shoulder_test_angle, 500);
    servo_base.setTargetAfter(0, servo_base_test_angle, 1000);
    servo_wrist.setTargetAfter(0, servo_wrist_test_angle, 1000);
    servo_rotate.setTargetAfter(0, servo_rotate_test_angle, 1000);
    servo_claw.setTargetAfter(0, servo_claw_test_angle, 1000);
    return 0;
}

void arm_script(int Command){
    switch (Command)
    {
    case 1:
        elbow.setTargetAngleAfter(0, 50, 1000);
        shoulder.setTargetAngleAfter(0, 70, 1000);

        elbow.setTargetAngleAfter(4000, 70, 1000);
        shoulder.setTargetAngleAfter(4000, 140, 1000);

        elbow.setTargetAngleAfter(6000, 90, 1000);
        shoulder.setTargetAngleAfter(6000, 175, 1000);
        
        shoulder.setTargetAngleAfter(5000, 180, 1000);
        servo_rotate.setTargetAfter(5000, 105, 1000);

        servo_claw.setTargetAfter(5000, 90, 1000);
        break;
    
    default:
        break;
    }
}

extern "C" 
{
    // 實作給 C 語言呼叫的橋接函式
    void cpp_arm_init(void) {
        arm_init(); // 呼叫你寫好的初始化
    }

    void cpp_arm_init_servo(void){
        arm_init_servo();
    }

    void cpp_arm_homing(void){
        arm_homing();
    }

    void cpp_arm_test(void) {
        arm_test(); // 呼叫你寫好的測試劇本
    }

    void cpp_arm_script(int command){
        arm_script(command);
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

