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
#define elbow_enc1_Pin GPIO_PIN_0
#define elbow_enc1_GPIO_Port GPIOF
#define elbow_enc2_Pin GPIO_PIN_1
#define elbow_enc2_GPIO_Port GPIOF
#define servo_base_pwm_Pin GPIO_PIN_0
#define servo_base_pwm_GPIO_Port GPIOA
#define servo_rotate_pwm_Pin GPIO_PIN_1
#define servo_rotate_pwm_GPIO_Port GPIOA
#define servo_claw_pwm_Pin GPIO_PIN_2
#define servo_claw_pwm_GPIO_Port GPIOA
#define servo_wrist_pwm_Pin GPIO_PIN_3
#define servo_wrist_pwm_GPIO_Port GPIOA
#define screen_dc_motor_dir_Pin GPIO_PIN_4
#define screen_dc_motor_dir_GPIO_Port GPIOA
#define pwm_200_Pin GPIO_PIN_6
#define pwm_200_GPIO_Port GPIOA
#define pwm_201_Pin GPIO_PIN_7
#define pwm_201_GPIO_Port GPIOA
#define test_light_Pin GPIO_PIN_0
#define test_light_GPIO_Port GPIOB
#define pwm_203_Pin GPIO_PIN_1
#define pwm_203_GPIO_Port GPIOB
#define limit_switch_rotate_Pin GPIO_PIN_2
#define limit_switch_rotate_GPIO_Port GPIOB
#define limit_switch_rotate_EXTI_IRQn EXTI2_IRQn
#define shoulder_enc1_Pin GPIO_PIN_11
#define shoulder_enc1_GPIO_Port GPIOF
#define shoulder_enc2_Pin GPIO_PIN_12
#define shoulder_enc2_GPIO_Port GPIOF
#define elbow_pwm_Pin GPIO_PIN_14
#define elbow_pwm_GPIO_Port GPIOB
#define shoulder_pwm_Pin GPIO_PIN_15
#define shoulder_pwm_GPIO_Port GPIOB
#define shoulder_dir_Pin GPIO_PIN_10
#define shoulder_dir_GPIO_Port GPIOD
#define elbow_dir_Pin GPIO_PIN_11
#define elbow_dir_GPIO_Port GPIOD
#define pwm_210_Pin GPIO_PIN_12
#define pwm_210_GPIO_Port GPIOD
#define fork_pwm_Pin GPIO_PIN_13
#define fork_pwm_GPIO_Port GPIOD
#define fork_dir_Pin GPIO_PIN_15
#define fork_dir_GPIO_Port GPIOD
#define shoulder_homing_switch_Pin GPIO_PIN_3
#define shoulder_homing_switch_GPIO_Port GPIOG
#define elbow_homing_switch_Pin GPIO_PIN_4
#define elbow_homing_switch_GPIO_Port GPIOG
#define fork_encoder_1_Pin GPIO_PIN_6
#define fork_encoder_1_GPIO_Port GPIOC
#define fork_encoder_2_Pin GPIO_PIN_7
#define fork_encoder_2_GPIO_Port GPIOC
#define pwm_202_Pin GPIO_PIN_8
#define pwm_202_GPIO_Port GPIOC
#define Trigger_Pin GPIO_PIN_1
#define Trigger_GPIO_Port GPIOD
#define Trigger_EXTI_IRQn EXTI1_IRQn
#define fork_lim_Pin GPIO_PIN_3
#define fork_lim_GPIO_Port GPIOD
#define fork_lim_EXTI_IRQn EXTI3_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
