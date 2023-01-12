/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

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
#define ADC1_INT_Pin GPIO_PIN_3
#define ADC1_INT_GPIO_Port GPIOA
#define ADC1_INT_EXTI_IRQn EXTI3_IRQn
#define ADC1_CS_Pin GPIO_PIN_4
#define ADC1_CS_GPIO_Port GPIOA
#define TC_CS4_Pin GPIO_PIN_1
#define TC_CS4_GPIO_Port GPIOB
#define TC_CS3_Pin GPIO_PIN_2
#define TC_CS3_GPIO_Port GPIOB
#define TC_CS2_Pin GPIO_PIN_12
#define TC_CS2_GPIO_Port GPIOB
#define TC_CS1_Pin GPIO_PIN_13
#define TC_CS1_GPIO_Port GPIOB
#define ADC2_CS_Pin GPIO_PIN_6
#define ADC2_CS_GPIO_Port GPIOB
#define ADC2_INT_Pin GPIO_PIN_7
#define ADC2_INT_GPIO_Port GPIOB
#define ADC2_INT_EXTI_IRQn EXTI9_5_IRQn
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
