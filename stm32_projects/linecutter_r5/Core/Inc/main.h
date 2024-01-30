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
#include "stm32wbxx_hal.h"

#include "app_conf.h"
#include "app_entry.h"
#include "app_common.h"

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
#define FLASH_CS_Pin GPIO_PIN_8
#define FLASH_CS_GPIO_Port GPIOB
#define USER_LED_Pin GPIO_PIN_9
#define USER_LED_GPIO_Port GPIOB
#define PYRO_SENSE_1_Pin GPIO_PIN_0
#define PYRO_SENSE_1_GPIO_Port GPIOA
#define PYRO_SENSE_2_Pin GPIO_PIN_1
#define PYRO_SENSE_2_GPIO_Port GPIOA
#define BATT_SENSE_IN_Pin GPIO_PIN_2
#define BATT_SENSE_IN_GPIO_Port GPIOA
#define BARO1_CS_Pin GPIO_PIN_3
#define BARO1_CS_GPIO_Port GPIOA
#define IMU1_CS_Pin GPIO_PIN_4
#define IMU1_CS_GPIO_Port GPIOA
#define LIGHT_SENSE_IN_Pin GPIO_PIN_8
#define LIGHT_SENSE_IN_GPIO_Port GPIOA
#define FIRE1_PWM_Pin GPIO_PIN_10
#define FIRE1_PWM_GPIO_Port GPIOA
#define FIRE2_PWM_Pin GPIO_PIN_15
#define FIRE2_PWM_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
