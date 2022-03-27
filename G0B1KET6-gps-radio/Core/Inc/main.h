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
#define RAD433_SCK_Pin GPIO_PIN_0
#define RAD433_SCK_GPIO_Port GPIOA
#define RAD433_CS_Pin GPIO_PIN_1
#define RAD433_CS_GPIO_Port GPIOA
#define RAD433_GP0_Pin GPIO_PIN_2
#define RAD433_GP0_GPIO_Port GPIOA
#define RAD433_MISO_Pin GPIO_PIN_3
#define RAD433_MISO_GPIO_Port GPIOA
#define RAD433_MOSI_Pin GPIO_PIN_4
#define RAD433_MOSI_GPIO_Port GPIOA
#define RAD433_GP2_Pin GPIO_PIN_5
#define RAD433_GP2_GPIO_Port GPIOA
#define RAD433_GP3_Pin GPIO_PIN_6
#define RAD433_GP3_GPIO_Port GPIOA
#define RAD433_RST_Pin GPIO_PIN_7
#define RAD433_RST_GPIO_Port GPIOA
#define GPS_TX_Pin GPIO_PIN_9
#define GPS_TX_GPIO_Port GPIOA
#define GPS_RX_Pin GPIO_PIN_10
#define GPS_RX_GPIO_Port GPIOA
#define BARO_CS_Pin GPIO_PIN_15
#define BARO_CS_GPIO_Port GPIOA
#define BARO_SCK_Pin GPIO_PIN_3
#define BARO_SCK_GPIO_Port GPIOB
#define BARO_MISO_Pin GPIO_PIN_4
#define BARO_MISO_GPIO_Port GPIOB
#define BARO_MOSI_Pin GPIO_PIN_5
#define BARO_MOSI_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define IWDG_HANDLE		&hiwdg
#define GPS_HUART 		&huart1
#define RAD433_HSPI 	&hspi2
#define BARO_HUART 		&hspi3
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
