#include "cmsis_os2.h"
#include "stm32h7xx_hal.h"
#include "servo.hpp"
#include "arm_test.hpp"
#include "encoder_dc.hpp"
#include "pid.hpp"
#include "limit_sw.hpp"
extern TIM_HandleTypeDef htim5, htim23, htim12, htim24; 
extern volatile bool Homing_arm;
extern volatile bool hay_bale_put;
volatile float test_p = 3.0f;
volatile float test_i = 0.0f;
volatile bool hay_bale_took = false;
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
    servo_wrist.attach(&htim5, TIM_CHANNEL_4, 7.3f, 500);
    // elbow.attach(&htim23, 26400.0f, &htim12, TIM_CHANNEL_1, GPIOD, GPIO_PIN_11);
    shoulder.attach(&htim24, 12720.0f, &htim12, TIM_CHANNEL_2, GPIOD, GPIO_PIN_10);

    arm_init_servo();

    return 0;
}
void homing()
{
    Homing_arm = true;

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

    elbow.homing_ccw(1800);

    while (!elbow_homing_switch.isPressed()) {
        osDelay(1); 
    }

    // 撞到了！立刻重置馬達計數，這裡就是絕對 0 度
    elbow.reset();
    is_homing_done[0] = true;

    Homing_arm = false;

}

volatile float elbow_test_angle = 8.0f;
volatile float shoulder_test_angle = 25.0f;
volatile float servo_base_test_angle = 35.0f;
volatile float servo_rotate_test_angle = 110.0f;
volatile float servo_claw_test_angle = 70.0f;
volatile float servo_wrist_test_angle = 5.0f;

void arm_homing(void){
    homing();

    osDelay(300);

    elbow.setTargetAngleAfter(0, 8.0f, 500);
    shoulder.setTargetAngleAfter(0, 25.0f, 500);
    servo_base.setTargetAfter( 500, 35.0f, 2000);

}

void arm_init_servo(void){
    servo_base.set_current_angle(125.0f);
    servo_wrist.set_current_angle(5.0f);
    servo_rotate.set_current_angle(110.0f);
    servo_claw.set_current_angle(70.0f);
    
    servo_base.setTargetAfter(0, 125.0f, 3000);
    servo_wrist.setTargetAfter(10, 5.0f, 3000);
    servo_rotate.setTargetAfter(0, 110.0f, 3000);
    servo_claw.setTargetAfter(0, 70.0f, 3000);
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

/*
look from the robot center
             10

          8     9

       5     6     7 

    1     2     3     4       
*/

void arm_script(int Command){
    /*
        shoulder motor max speed    :    96 deg/s
        elbow motor max speed       :    60 deg/s
    */
    switch (Command)
    {
    case 99: // take hay bale
        hay_bale_took = false;
        hay_bale_put = false;
        
        elbow.setTargetAngleAfter(0, 50, 1000);
        shoulder.setTargetAngleAfter(0, 70, 1000);

        elbow.setTargetAngleAfter(1000, 70, 1000);
        shoulder.setTargetAngleAfter(1000, 140, 1000);
        servo_claw.setTargetAfter(1000, 40, 1000);

        osDelay(2000);

        elbow.setTargetAngleAfter(1000, 100, 1000);
        shoulder.setTargetAngleAfter(1000, 175, 1000);

        shoulder.setTargetAngleAfter(2000, 183, 1000);
        servo_rotate.setTargetAfter(2000, 95, 1000);


        osDelay(3000);

        servo_claw.setTargetAfter(0, 96, 1000);

        hay_bale_took = true;
        break;

    case 11: // from fork to hay bale pos
        shoulder.setTargetAngleAfter(0, 90, 2000);
        elbow.setTargetAngleAfter(0, 100, 2000);
        servo_claw.setTargetAfter(0, 70, 2000);
        servo_rotate.setTargetAfter(0, 110,2000);
        servo_base.setTargetAfter(2000, 35, 2000);

        osDelay(4000);

        shoulder.setTargetAngleAfter(0, 25, 500);
        elbow.setTargetAngleAfter(0, 8, 500);
        break;

    case 1: // put to the 1 pos
        shoulder.setTargetAngleAfter(0, 90, 1000);

        servo_base.setTargetAfter(2000, 180, 2000);

        osDelay(4000);

        servo_rotate.setTargetAfter(0, 130, 1000);
        elbow.setTargetAngleAfter(0, 60, 1000);
        shoulder.setTargetAngleAfter(0, 110, 1000);


        servo_claw.setTargetAfter(1000, 85, 500);

        hay_bale_put = true;
        break;

    case 2: // put to the 2 pos
        break;
    case 3: // put to the 3 pos
        break;
    case 4: // put to the 4 pos
        break;
    case 5: // put to the 5 pos
        break;
    case 6: // put to the 6 pos
        break;
    case 7: // put to the 7 pos
        break;
    case 8: // put to the 8 pos
        break;
    case 9: // put to the 9 pos
        break;
    case 10: // put to the 10 pos
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

