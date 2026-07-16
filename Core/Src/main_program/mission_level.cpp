#include "mission_level.hpp"
#include "cmsis_os2.h"
#include "level_config.hpp"
#include "stm32h7xx_hal.h"

volatile int mission_level = 0;
int last_mission_level = 0;
bool task_created = false;


void mission_control()
{
    if (last_mission_level != mission_level)
    {
        last_mission_level = mission_level;
        switch (mission_level)
        {
        case 1:
        if(!task_created){
            task_created = true;
            level_1 = osThreadNew(Startlevel_1, NULL, &level_1_t);
        }
            break;

        case 2:
        if(!task_created){
            task_created = true;
            level_2 = osThreadNew(Startlevel_2, NULL, &level_2_t);
        }
            break;
        case 3:
        if(!task_created){
            task_created = true;
            level_3 = osThreadNew(Startlevel_3, NULL, &level_3_t);
        }
            break;
        default:
            break;
        }
    }
}

void Startlevel_1(void *argument)
{
    for (;;)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
        osDelay(1000);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
        osDelay(1000);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
        osDelay(1000);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
        osDelay(1);
    }
    osThreadExit();
}

void Startlevel_2(void *argument)
{
    osDelay(100);
    osThreadExit();
}

void Startlevel_3(void *argument)
{
    osDelay(100);
    osThreadExit();
}
