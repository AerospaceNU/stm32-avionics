/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "stm32h7xx_it.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define IMU1_AG_HSPI		&hspi3
#define IMU1_AG_CS_PORT 	GPIOD
#define IMU1_AG_CS_PIN		GPIO_PIN_1
#define IMU1_M_HSPI			&hspi3
#define IMU1_M_CS_PORT 		GPIOD
#define IMU1_M_CS_PIN		GPIO_PIN_0

#define IMU2_AG_HSPI		&hspi6
#define IMU2_AG_CS_PORT 	GPIOE
#define IMU2_AG_CS_PIN		GPIO_PIN_1
#define IMU2_M_HSPI			&hspi6
#define IMU2_M_CS_PORT 		GPIOE
#define IMU2_M_CS_PIN		GPIO_PIN_0

#define BARO1_HSPI			&hspi3
#define BARO1_CS_PORT		GPIOA
#define BARO1_CS_PIN		GPIO_PIN_15

#define BARO2_HSPI			&hspi6
#define BARO2_CS_PORT		GPIOE
#define BARO2_CS_PIN		GPIO_PIN_2

#define HIGH_G_HSPI			&hspi3
#define HIGH_G_CS_PORT		GPIOD
#define HIGH_G_CS_PIN		GPIO_PIN_2

#define SERVO1_HTIM			&htim8
#define SERVO1_CHANNEL		TIM_CHANNEL_1

#define SERVO2_HTIM			&htim8
#define SERVO2_CHANNEL		TIM_CHANNEL_2

#define SERVO3_HTIM			&htim8
#define SERVO3_CHANNEL		TIM_CHANNEL_3

#define SERVO4_HTIM			&htim8
#define SERVO4_CHANNEL		TIM_CHANNEL_4

#define BUZZER_HTIM			&htim1
#define BUZZER_CHANNEL		TIM_CHANNEL_1

#define FLASH_HSPI			&hspi1
#define FLASH_CS_PORT		GPIOC
#define FLASH_CS_PIN		GPIO_PIN_5

#define LED1_PORT			GPIOE
#define LED1_PIN			GPIO_PIN_8

#define GPS_HUART			&huart5

#define RADIO_HSPI			&hspi4
#define RADIO_CS_PORT		GPIOE
#define RADIO_CS_PIN		GPIO_PIN_11
#define RADIO_RST_PORT		GPIOB
#define RADIO_RST_PIN		GPIO_PIN_15
#define RADIO_RDY_PORT		GPIOE
#define RADIO_RDY_PIN		GPIO_PIN_13
#define RADIO_GP0_PORT		GPIOE
#define RADIO_GP0_PIN		GPIO_PIN_15
#define RADIO_GP2_PORT		GPIOB
#define RADIO_GP2_PIN		GPIO_PIN_10
#define RADIO_GP3_PORT		GPIOB
#define RADIO_GP3_PIN		GPIO_PIN_11

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
