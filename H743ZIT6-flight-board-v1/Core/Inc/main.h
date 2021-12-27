/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#define IMU2_CS_Pin GPIO_PIN_3
#define IMU2_CS_GPIO_Port GPIOE
#define BARO2_CS_Pin GPIO_PIN_4
#define BARO2_CS_GPIO_Port GPIOE
#define SENSE_SDA_Pin GPIO_PIN_0
#define SENSE_SDA_GPIO_Port GPIOF
#define SENSE_SCL_Pin GPIO_PIN_1
#define SENSE_SCL_GPIO_Port GPIOF
#define SENSE_ALERT_Pin GPIO_PIN_2
#define SENSE_ALERT_GPIO_Port GPIOF
#define BLUETOOTH_CS_Pin GPIO_PIN_10
#define BLUETOOTH_CS_GPIO_Port GPIOF
#define GPS_TX_Pin GPIO_PIN_0
#define GPS_TX_GPIO_Port GPIOA
#define GPS_RX_Pin GPIO_PIN_1
#define GPS_RX_GPIO_Port GPIOA
#define GPS_INT_Pin GPIO_PIN_2
#define GPS_INT_GPIO_Port GPIOA
#define AIN_PYRO_V_Pin GPIO_PIN_3
#define AIN_PYRO_V_GPIO_Port GPIOA
#define FLASH_CS_Pin GPIO_PIN_4
#define FLASH_CS_GPIO_Port GPIOA
#define AIN_PYRO1_Pin GPIO_PIN_4
#define AIN_PYRO1_GPIO_Port GPIOC
#define AIN_PYRO2_Pin GPIO_PIN_5
#define AIN_PYRO2_GPIO_Port GPIOC
#define AIN_PYRO3_Pin GPIO_PIN_0
#define AIN_PYRO3_GPIO_Port GPIOB
#define AIN_PYRO4_Pin GPIO_PIN_1
#define AIN_PYRO4_GPIO_Port GPIOB
#define AIN_PYRO5_Pin GPIO_PIN_11
#define AIN_PYRO5_GPIO_Port GPIOF
#define AIN_PYRO6_Pin GPIO_PIN_12
#define AIN_PYRO6_GPIO_Port GPIOF
#define FIRE6_Pin GPIO_PIN_13
#define FIRE6_GPIO_Port GPIOF
#define FIRE5_Pin GPIO_PIN_14
#define FIRE5_GPIO_Port GPIOF
#define FIRE4_Pin GPIO_PIN_15
#define FIRE4_GPIO_Port GPIOF
#define FIRE3_Pin GPIO_PIN_0
#define FIRE3_GPIO_Port GPIOG
#define FIRE2_Pin GPIO_PIN_1
#define FIRE2_GPIO_Port GPIOG
#define FIRE1_Pin GPIO_PIN_7
#define FIRE1_GPIO_Port GPIOE
#define PPS_Pin GPIO_PIN_9
#define PPS_GPIO_Port GPIOE
#define LED1_Pin GPIO_PIN_10
#define LED1_GPIO_Port GPIOE
#define LED2_Pin GPIO_PIN_11
#define LED2_GPIO_Port GPIOE
#define LED3_Pin GPIO_PIN_12
#define LED3_GPIO_Port GPIOE
#define LED4_Pin GPIO_PIN_13
#define LED4_GPIO_Port GPIOE
#define LED5_Pin GPIO_PIN_14
#define LED5_GPIO_Port GPIOE
#define RADIO_CS_Pin GPIO_PIN_12
#define RADIO_CS_GPIO_Port GPIOB
#define RADIO_GP2_Pin GPIO_PIN_8
#define RADIO_GP2_GPIO_Port GPIOD
#define RADIO_GP0_Pin GPIO_PIN_9
#define RADIO_GP0_GPIO_Port GPIOD
#define RADIO_GP3_Pin GPIO_PIN_10
#define RADIO_GP3_GPIO_Port GPIOD
#define RADIO_RST_Pin GPIO_PIN_11
#define RADIO_RST_GPIO_Port GPIOD
#define HGM_Pin GPIO_PIN_12
#define HGM_GPIO_Port GPIOD
#define SERVO_EN_Pin GPIO_PIN_8
#define SERVO_EN_GPIO_Port GPIOG
#define SERVO4_Pin GPIO_PIN_6
#define SERVO4_GPIO_Port GPIOC
#define SERVO3_Pin GPIO_PIN_7
#define SERVO3_GPIO_Port GPIOC
#define SERVO2_Pin GPIO_PIN_8
#define SERVO2_GPIO_Port GPIOC
#define SERVO1_Pin GPIO_PIN_9
#define SERVO1_GPIO_Port GPIOC
#define BUZZER_Pin GPIO_PIN_8
#define BUZZER_GPIO_Port GPIOA
#define BARO1_CS_Pin GPIO_PIN_15
#define BARO1_CS_GPIO_Port GPIOA
#define HIGH_G_INT1_Pin GPIO_PIN_0
#define HIGH_G_INT1_GPIO_Port GPIOD
#define HIGH_G_INT2_Pin GPIO_PIN_1
#define HIGH_G_INT2_GPIO_Port GPIOD
#define HIGH_G_CS_Pin GPIO_PIN_2
#define HIGH_G_CS_GPIO_Port GPIOD
#define IMU1_AG_INT2_Pin GPIO_PIN_4
#define IMU1_AG_INT2_GPIO_Port GPIOD
#define IMU1_AG_INT1_Pin GPIO_PIN_5
#define IMU1_AG_INT1_GPIO_Port GPIOD
#define IMU1_M_INT_Pin GPIO_PIN_6
#define IMU1_M_INT_GPIO_Port GPIOD
#define IMU1_M_DRDY_Pin GPIO_PIN_7
#define IMU1_M_DRDY_GPIO_Port GPIOD
#define IMU1_M_CS_Pin GPIO_PIN_9
#define IMU1_M_CS_GPIO_Port GPIOG
#define IMU1_AG_CS_Pin GPIO_PIN_10
#define IMU1_AG_CS_GPIO_Port GPIOG
#define IMU2_INT_Pin GPIO_PIN_4
#define IMU2_INT_GPIO_Port GPIOB
#define IMU2_FSYNC_Pin GPIO_PIN_5
#define IMU2_FSYNC_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define IMU1_AG_HSPI		&hspi3
#define IMU1_M_HSPI			&hspi3

#define IMU2_HSPI			&hspi4

#define BARO1_HSPI			&hspi3

#define BARO2_HSPI			&hspi4

#define HIGH_G_HSPI			&hspi3

#define SERVO1_HTIM			&htim8
#define SERVO1_CHANNEL		TIM_CHANNEL_4

#define SERVO2_HTIM			&htim8
#define SERVO2_CHANNEL		TIM_CHANNEL_3

#define SERVO3_HTIM			&htim8
#define SERVO3_CHANNEL		TIM_CHANNEL_2

#define SERVO4_HTIM			&htim8
#define SERVO4_CHANNEL		TIM_CHANNEL_1

#define BUZZER_HTIM			&htim1
#define BUZZER_CHANNEL		TIM_CHANNEL_1

#define FLASH_HSPI			&hspi1

#define GPS_HUART			&huart4

#define RADIO_HSPI			&hspi2
#define RADIO_RDY_GPIO_Port GPIOB
#define RADIO_RDY_Pin		GPIO_PIN_14

#define PYRO1_ADC			&hadc1
#define PYRO1_ADC_RANK		2

#define PYRO2_ADC			&hadc1
#define PYRO2_ADC_RANK		5

#define PYRO3_ADC			&hadc1
#define PYRO3_ADC_RANK		6

#define PYRO4_ADC			&hadc1
#define PYRO4_ADC_RANK		3

#define PYRO5_ADC			&hadc1
#define PYRO5_ADC_RANK		1

#define PYRO6_ADC			&hadc1
#define PYRO6_ADC_RANK		4
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
