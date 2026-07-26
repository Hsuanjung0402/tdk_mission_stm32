#ifndef LIMIT_SW_HPP
#define LIMIT_SW_HPP

#include "stm32h7xx_hal.h"

class LimitSwitch {
private:
    GPIO_TypeDef* port;
    uint16_t pin;
    uint32_t debounce_delay;
    volatile uint32_t last_interrupt_time; // 紀錄上次觸發中斷的時間 (必須加 volatile)

public:
    // 建構子
    LimitSwitch(GPIO_TypeDef* port, uint16_t pin, uint32_t debounce_delay_ms = 20);

    // 給中斷處理函式呼叫的防彈跳檢查
    bool checkInterrupt();

    // 取得這顆開關的硬體腳位 (方便在中斷回呼時比對)
    uint16_t getPin() const { return pin; }
    
    // 即時讀取狀態 (輔助用)
    bool isPressed();
};

#endif // LIMIT_SW_HPP