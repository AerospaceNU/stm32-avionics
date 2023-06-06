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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BARO1_CS_Pin GPIO_PIN_3
#define BARO1_CS_GPIO_Port GPIOE
#define VAPC_Pin GPIO_PIN_4
#define VAPC_GPIO_Port GPIOA
#define LNA433_EN_Pin GPIO_PIN_6
#define LNA433_EN_GPIO_Port GPIOA
#define RS485_DE_Pin GPIO_PIN_9
#define RS485_DE_GPIO_Port GPIOE
#define RS485_RE_Pin GPIO_PIN_10
#define RS485_RE_GPIO_Port GPIOE
#define RAD433_CS_Pin GPIO_PIN_15
#define RAD433_CS_GPIO_Port GPIOA
#define RAD433_SCK_Pin GPIO_PIN_10
#define RAD433_SCK_GPIO_Port GPIOC
#define RAD433_MISO_Pin GPIO_PIN_11
#define RAD433_MISO_GPIO_Port GPIOC
#define RAD433_MOSI_Pin GPIO_PIN_12
#define RAD433_MOSI_GPIO_Port GPIOC
#define RAD433_GP0_Pin GPIO_PIN_0
#define RAD433_GP0_GPIO_Port GPIOD
#define RAD433_GP2_Pin GPIO_PIN_1
#define RAD433_GP2_GPIO_Port GPIOD
#define RAD433_GP3_Pin GPIO_PIN_2
#define RAD433_GP3_GPIO_Port GPIOD
#define RAD433_RST_Pin GPIO_PIN_3
#define RAD433_RST_GPIO_Port GPIOD
#define RAD915_CS_Pin GPIO_PIN_4
#define RAD915_CS_GPIO_Port GPIOD
#define RAD915_GP0_Pin GPIO_PIN_5
#define RAD915_GP0_GPIO_Port GPIOD
#define RAD915_GP2_Pin GPIO_PIN_6
#define RAD915_GP2_GPIO_Port GPIOD
#define RAD915_MISO_Pin GPIO_PIN_4
#define RAD915_MISO_GPIO_Port GPIOB
#define RAD915_GP3_Pin GPIO_PIN_5
#define RAD915_GP3_GPIO_Port GPIOB
#define RAD915_RST_Pin GPIO_PIN_6
#define RAD915_RST_GPIO_Port GPIOB
#define GPS_INT_Pin GPIO_PIN_8
#define GPS_INT_GPIO_Port GPIOB
#define GPS_PPS_Pin GPIO_PIN_9
#define GPS_PPS_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
