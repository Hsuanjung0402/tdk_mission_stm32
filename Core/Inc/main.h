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
#define screen_pink_pwm_Pin GPIO_PIN_5
#define screen_pink_pwm_GPIO_Port GPIOE
#define screen_blue_pwm_Pin GPIO_PIN_6
#define screen_blue_pwm_GPIO_Port GPIOE
#define elbow_pwm_Pin GPIO_PIN_1
#define elbow_pwm_GPIO_Port GPIOF
#define shoulder_pwm_Pin GPIO_PIN_2
#define shoulder_pwm_GPIO_Port GPIOF
#define fork_pwm_Pin GPIO_PIN_3
#define fork_pwm_GPIO_Port GPIOF
#define screen_pwm_Pin GPIO_PIN_8
#define screen_pwm_GPIO_Port GPIOF
#define screen_dc_motor_dir_Pin GPIO_PIN_4
#define screen_dc_motor_dir_GPIO_Port GPIOA
#define elbow_enc_1_Pin GPIO_PIN_6
#define elbow_enc_1_GPIO_Port GPIOA
#define elbow_enc_2_Pin GPIO_PIN_7
#define elbow_enc_2_GPIO_Port GPIOA
#define test_light_Pin GPIO_PIN_0
#define test_light_GPIO_Port GPIOB
#define limit_switch_rotate_Pin GPIO_PIN_2
#define limit_switch_rotate_GPIO_Port GPIOB
#define limit_switch_rotate_EXTI_IRQn EXTI2_IRQn
#define claw_pwm_Pin GPIO_PIN_11
#define claw_pwm_GPIO_Port GPIOF
#define rotatae_pwm_Pin GPIO_PIN_12
#define rotatae_pwm_GPIO_Port GPIOF
#define wrist_pwm_Pin GPIO_PIN_13
#define wrist_pwm_GPIO_Port GPIOF
#define base_pwm_Pin GPIO_PIN_14
#define base_pwm_GPIO_Port GPIOF
#define elbow_dir_Pin GPIO_PIN_1
#define elbow_dir_GPIO_Port GPIOG
#define pusher_second_pwm_Pin GPIO_PIN_14
#define pusher_second_pwm_GPIO_Port GPIOB
#define pusher_first_pwm_Pin GPIO_PIN_15
#define pusher_first_pwm_GPIO_Port GPIOB
#define shoulder_limit_Pin GPIO_PIN_10
#define shoulder_limit_GPIO_Port GPIOD
#define trigger_limit_Pin GPIO_PIN_11
#define trigger_limit_GPIO_Port GPIOD
#define trigger_limit_EXTI_IRQn EXTI15_10_IRQn
#define shoulder_enc_1_Pin GPIO_PIN_12
#define shoulder_enc_1_GPIO_Port GPIOD
#define shoulder_enc_2_Pin GPIO_PIN_13
#define shoulder_enc_2_GPIO_Port GPIOD
#define elbow_limit_Pin GPIO_PIN_14
#define elbow_limit_GPIO_Port GPIOD
#define fork_limit_Pin GPIO_PIN_15
#define fork_limit_GPIO_Port GPIOD
#define fork_limit_EXTI_IRQn EXTI15_10_IRQn
#define shoulder_dir_Pin GPIO_PIN_2
#define shoulder_dir_GPIO_Port GPIOG
#define fork_dir_Pin GPIO_PIN_3
#define fork_dir_GPIO_Port GPIOG
#define screen_dir_Pin GPIO_PIN_4
#define screen_dir_GPIO_Port GPIOG
#define fork_enc_1_Pin GPIO_PIN_6
#define fork_enc_1_GPIO_Port GPIOC
#define fork_enc_2_Pin GPIO_PIN_7
#define fork_enc_2_GPIO_Port GPIOC

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
