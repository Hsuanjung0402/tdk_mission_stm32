#include "limit_sw.hpp"

LimitSwitch::LimitSwitch(GPIO_TypeDef* port, uint16_t pin, uint32_t debounce_delay_ms)
    : port(port), pin(pin), debounce_delay(debounce_delay_ms)
{
    last_interrupt_time = 0;
}

// 👉 專門放在 EXTI_Callback 裡面的防彈跳檢查
bool LimitSwitch::checkInterrupt() {
    uint32_t current_time = HAL_GetTick();
    
    // 如果距離上一次觸發的時間大於防彈跳時間，才算是有效觸發
    if ((current_time - last_interrupt_time) > debounce_delay) {
        last_interrupt_time = current_time;
        return true;  // 有效按下！
    }
    return false; // 雜訊，忽略
}

// 輔助函式：即時讀取腳位狀態 (假設是 Pull-up，按下為 LOW)
bool LimitSwitch::isPressed() {
    return (HAL_GPIO_ReadPin(port, pin) == GPIO_PIN_RESET);
}