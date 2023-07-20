/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32wlxx_hal.h"

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
#define GPS_TX_Pin GPIO_PIN_6
#define GPS_TX_GPIO_Port GPIOB
#define GPS_RX_Pin GPIO_PIN_7
#define GPS_RX_GPIO_Port GPIOB
#define BARO_CS_Pin GPIO_PIN_8
#define BARO_CS_GPIO_Port GPIOB
#define PWM_OUT1_Pin GPIO_PIN_0
#define PWM_OUT1_GPIO_Port GPIOA
#define BUZZER_Pin GPIO_PIN_1
#define BUZZER_GPIO_Port GPIOA
#define SERIAL_TX_Pin GPIO_PIN_2
#define SERIAL_TX_GPIO_Port GPIOA
#define SERIAL_RX_Pin GPIO_PIN_3
#define SERIAL_RX_GPIO_Port GPIOA
#define USER_LED_Pin GPIO_PIN_4
#define USER_LED_GPIO_Port GPIOA
#define FLASH_SCK_Pin GPIO_PIN_5
#define FLASH_SCK_GPIO_Port GPIOA
#define FLASH_MISO_Pin GPIO_PIN_6
#define FLASH_MISO_GPIO_Port GPIOA
#define FLASH_MOSI_Pin GPIO_PIN_7
#define FLASH_MOSI_GPIO_Port GPIOA
#define PWM_OUT0_Pin GPIO_PIN_8
#define PWM_OUT0_GPIO_Port GPIOA
#define FRAM_CS_Pin GPIO_PIN_9
#define FRAM_CS_GPIO_Port GPIOA
#define FLASH_CS_Pin GPIO_PIN_12
#define FLASH_CS_GPIO_Port GPIOB
#define IMU_CS_Pin GPIO_PIN_10
#define IMU_CS_GPIO_Port GPIOA
#define TRX_SW_Pin GPIO_PIN_13
#define TRX_SW_GPIO_Port GPIOC
#define FIRE_3_Pin GPIO_PIN_15
#define FIRE_3_GPIO_Port GPIOA
void   MX_SUBGHZ_Init(void);
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
