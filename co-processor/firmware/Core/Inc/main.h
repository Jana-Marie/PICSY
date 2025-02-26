/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32g0xx_hal.h"

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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define VBAT_Pin GPIO_PIN_0
#define VBAT_GPIO_Port GPIOA
#define DITHER_Pin GPIO_PIN_1
#define DITHER_GPIO_Port GPIOA
#define PALETTE_Pin GPIO_PIN_2
#define PALETTE_GPIO_Port GPIOA
#define EXPOSURE_Pin GPIO_PIN_3
#define EXPOSURE_GPIO_Port GPIOA
#define TRIGGER_A_Pin GPIO_PIN_4
#define TRIGGER_A_GPIO_Port GPIOA
#define TRIGGER_A_EXTI_IRQn EXTI4_15_IRQn
#define TRIGGER_B_Pin GPIO_PIN_5
#define TRIGGER_B_GPIO_Port GPIOA
#define TRIGGER_B_EXTI_IRQn EXTI4_15_IRQn
#define DISPLAY_1_CS_Pin GPIO_PIN_6
#define DISPLAY_1_CS_GPIO_Port GPIOA
#define DISPLAY_2_CS_Pin GPIO_PIN_7
#define DISPLAY_2_CS_GPIO_Port GPIOA
#define DISPLAY_RST_Pin GPIO_PIN_0
#define DISPLAY_RST_GPIO_Port GPIOB
#define INT_OUT_Pin GPIO_PIN_3
#define INT_OUT_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
