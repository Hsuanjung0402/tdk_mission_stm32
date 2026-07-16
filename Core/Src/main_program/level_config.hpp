#include "cmsis_os2.h"

#ifdef __cplusplus
extern "C"{
#endif

void Startlevel_1(void *argument);
void Startlevel_2(void *argument);
void Startlevel_3(void *argument);

#ifdef __cplusplus
}
#endif


osThreadId_t level_1 = NULL;
osThreadId_t level_2 = NULL;
osThreadId_t level_3 = NULL;

const osThreadAttr_t level_1_t = {
    .name = "level_1",
    .stack_size = 256 * 4,          
    .priority = (osPriority_t) osPriorityNormal,
};

const osThreadAttr_t level_2_t = {
    .name = "level_2",
    .stack_size = 256 * 4,          
    .priority = (osPriority_t) osPriorityNormal,
};

const osThreadAttr_t level_3_t = {
    .name = "level_3",
    .stack_size = 256 * 4,          
    .priority = (osPriority_t) osPriorityNormal,
};