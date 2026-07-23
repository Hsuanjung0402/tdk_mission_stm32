/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define dc_motor_screen_1_Pin GPIO_PIN_1
#define dc_motor_screen_1_GPIO_Port GPIOA
#define dc_motor_screen_2_Pin GPIO_PIN_4
#define dc_motor_screen_2_GPIO_Port GPIOA
#define TIANKONGRC_11kg_1_Pin GPIO_PIN_6
#define TIANKONGRC_11kg_1_GPIO_Port GPIOA
#define Gobilda_300_torque_Pin GPIO_PIN_7
#define Gobilda_300_torque_GPIO_Port GPIOA
#define Limit_Switch_Pin GPIO_PIN_2
#define Limit_Switch_GPIO_Port GPIOB
#define Limit_Switch_EXTI_IRQn EXTI2_IRQn
#define TIANKONGRC_11kg_2_Pin GPIO_PIN_8
#define TIANKONGRC_11kg_2_GPIO_Port GPIOC

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
