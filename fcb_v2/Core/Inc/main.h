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
#include "stm32l4xx_hal.h"

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
#define PERIPH_EN_Pin GPIO_PIN_2
#define PERIPH_EN_GPIO_Port GPIOE
#define SERVO4_Pin GPIO_PIN_3
#define SERVO4_GPIO_Port GPIOE
#define SERVO3_Pin GPIO_PIN_4
#define SERVO3_GPIO_Port GPIOE
#define SERVO2_Pin GPIO_PIN_5
#define SERVO2_GPIO_Port GPIOE
#define SERVO1_Pin GPIO_PIN_6
#define SERVO1_GPIO_Port GPIOE
#define PYRO1_SENSE_Pin GPIO_PIN_2
#define PYRO1_SENSE_GPIO_Port GPIOC
#define SYS_BATT_SENSE_Pin GPIO_PIN_3
#define SYS_BATT_SENSE_GPIO_Port GPIOC
#define FIRE1_Pin GPIO_PIN_0
#define FIRE1_GPIO_Port GPIOA
#define IMU2_INT_Pin GPIO_PIN_4
#define IMU2_INT_GPIO_Port GPIOA
#define IMU2_CS_Pin GPIO_PIN_5
#define IMU2_CS_GPIO_Port GPIOA
#define PYRO2_SENSE_Pin GPIO_PIN_4
#define PYRO2_SENSE_GPIO_Port GPIOC
#define PYRO4_SENSE_Pin GPIO_PIN_5
#define PYRO4_SENSE_GPIO_Port GPIOC
#define PYRO_BATT_SENSE_Pin GPIO_PIN_0
#define PYRO_BATT_SENSE_GPIO_Port GPIOB
#define PYRO3_SENSE_Pin GPIO_PIN_1
#define PYRO3_SENSE_GPIO_Port GPIOB
#define USER_LED_Pin GPIO_PIN_8
#define USER_LED_GPIO_Port GPIOE
#define FIRE2_Pin GPIO_PIN_9
#define FIRE2_GPIO_Port GPIOE
#define IMU1_CS_Pin GPIO_PIN_10
#define IMU1_CS_GPIO_Port GPIOE
#define FIRE3_Pin GPIO_PIN_11
#define FIRE3_GPIO_Port GPIOE
#define HIGH_G_CS_Pin GPIO_PIN_12
#define HIGH_G_CS_GPIO_Port GPIOE
#define FIRE4_Pin GPIO_PIN_13
#define FIRE4_GPIO_Port GPIOE
#define IMU1_INT_Pin GPIO_PIN_14
#define IMU1_INT_GPIO_Port GPIOE
#define HIGH_G_INT_Pin GPIO_PIN_15
#define HIGH_G_INT_GPIO_Port GPIOE
#define RFFE_SCL_Pin GPIO_PIN_10
#define RFFE_SCL_GPIO_Port GPIOB
#define RFFE_SDA_Pin GPIO_PIN_11
#define RFFE_SDA_GPIO_Port GPIOB
#define GPS_PPS_Pin GPIO_PIN_12
#define GPS_PPS_GPIO_Port GPIOB
#define GPS_TX_Pin GPIO_PIN_8
#define GPS_TX_GPIO_Port GPIOD
#define GPS_RX_Pin GPIO_PIN_9
#define GPS_RX_GPIO_Port GPIOD
#define BARO1_CS_Pin GPIO_PIN_10
#define BARO1_CS_GPIO_Port GPIOD
#define FRAM_CS_Pin GPIO_PIN_11
#define FRAM_CS_GPIO_Port GPIOD
#define RFFE_TRX_SW_Pin GPIO_PIN_12
#define RFFE_TRX_SW_GPIO_Port GPIOD
#define RFFE_EN_Pin GPIO_PIN_13
#define RFFE_EN_GPIO_Port GPIOD
#define RFFE_BYP_Pin GPIO_PIN_14
#define RFFE_BYP_GPIO_Port GPIOD
#define BARO2_CS_Pin GPIO_PIN_8
#define BARO2_CS_GPIO_Port GPIOA
#define RAD433_SCK_Pin GPIO_PIN_10
#define RAD433_SCK_GPIO_Port GPIOC
#define RAD433_MISO_Pin GPIO_PIN_11
#define RAD433_MISO_GPIO_Port GPIOC
#define RAD433_MOSI_Pin GPIO_PIN_12
#define RAD433_MOSI_GPIO_Port GPIOC
#define RAD433_CS_Pin GPIO_PIN_0
#define RAD433_CS_GPIO_Port GPIOD
#define RAD433_IO0_Pin GPIO_PIN_1
#define RAD433_IO0_GPIO_Port GPIOD
#define RAD433_IO2_Pin GPIO_PIN_2
#define RAD433_IO2_GPIO_Port GPIOD
#define RAD433_IO3_Pin GPIO_PIN_3
#define RAD433_IO3_GPIO_Port GPIOD
#define RAD433_RST_Pin GPIO_PIN_4
#define RAD433_RST_GPIO_Port GPIOD
#define MAG_INT_Pin GPIO_PIN_7
#define MAG_INT_GPIO_Port GPIOD
#define MAG_SCL_Pin GPIO_PIN_6
#define MAG_SCL_GPIO_Port GPIOB
#define MAG_SDA_Pin GPIO_PIN_7
#define MAG_SDA_GPIO_Port GPIOB
#define FLASH_CS_Pin GPIO_PIN_0
#define FLASH_CS_GPIO_Port GPIOE
#define BUZZER_Pin GPIO_PIN_1
#define BUZZER_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
