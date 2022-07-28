/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f1xx_hal.h"

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
#define pin_kv2_Pin GPIO_PIN_13
#define pin_kv2_GPIO_Port GPIOC
#define ADC_volt_Pin GPIO_PIN_0
#define ADC_volt_GPIO_Port GPIOA
#define pin_CS_Pin GPIO_PIN_1
#define pin_CS_GPIO_Port GPIOA
#define pin_enc_Pin GPIO_PIN_2
#define pin_enc_GPIO_Port GPIOA
#define pin_ur_led_Pin GPIO_PIN_3
#define pin_ur_led_GPIO_Port GPIOA
#define pin_sol1_Pin GPIO_PIN_4
#define pin_sol1_GPIO_Port GPIOA
#define pin_sol2_Pin GPIO_PIN_5
#define pin_sol2_GPIO_Port GPIOA
#define pin_sol3_Pin GPIO_PIN_6
#define pin_sol3_GPIO_Port GPIOA
#define pin_sol4_Pin GPIO_PIN_7
#define pin_sol4_GPIO_Port GPIOA
#define pin_dv41_Pin GPIO_PIN_0
#define pin_dv41_GPIO_Port GPIOB
#define pin_dv42_Pin GPIO_PIN_1
#define pin_dv42_GPIO_Port GPIOB
#define pin_dv31_Pin GPIO_PIN_10
#define pin_dv31_GPIO_Port GPIOB
#define pin_dv32_Pin GPIO_PIN_11
#define pin_dv32_GPIO_Port GPIOB
#define pin_shag_en_Pin GPIO_PIN_12
#define pin_shag_en_GPIO_Port GPIOB
#define pin_shag_st_Pin GPIO_PIN_13
#define pin_shag_st_GPIO_Port GPIOB
#define pin_shag_dir_Pin GPIO_PIN_14
#define pin_shag_dir_GPIO_Port GPIOB
#define pin_dv22_Pin GPIO_PIN_15
#define pin_dv22_GPIO_Port GPIOB
#define pin_dv21_Pin GPIO_PIN_8
#define pin_dv21_GPIO_Port GPIOA
#define pin_dv11_Pin GPIO_PIN_9
#define pin_dv11_GPIO_Port GPIOA
#define pin_dv12_Pin GPIO_PIN_10
#define pin_dv12_GPIO_Port GPIOA
#define pin_kv1_Pin GPIO_PIN_15
#define pin_kv1_GPIO_Port GPIOA
#define PWM_KD_Pin GPIO_PIN_4
#define PWM_KD_GPIO_Port GPIOB
#define pin_kv3_Pin GPIO_PIN_5
#define pin_kv3_GPIO_Port GPIOB
#define pin_kv4_Pin GPIO_PIN_6
#define pin_kv4_GPIO_Port GPIOB
#define Res_USB_Pin GPIO_PIN_7
#define Res_USB_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
