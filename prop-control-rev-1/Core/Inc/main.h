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
#include "stm32g4xx_hal.h"

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
#define SOL_1_Pin GPIO_PIN_13
#define SOL_1_GPIO_Port GPIOC
#define SOL_2_Pin GPIO_PIN_14
#define SOL_2_GPIO_Port GPIOC
#define SOL_3_Pin GPIO_PIN_15
#define SOL_3_GPIO_Port GPIOC
#define ADCMUX1_Pin GPIO_PIN_0
#define ADCMUX1_GPIO_Port GPIOA
#define ADCMUX2_Pin GPIO_PIN_1
#define ADCMUX2_GPIO_Port GPIOA
#define ADCMUX3_Pin GPIO_PIN_2
#define ADCMUX3_GPIO_Port GPIOA
#define ADC_SEL_A_Pin GPIO_PIN_3
#define ADC_SEL_A_GPIO_Port GPIOA
#define ADC_SEL_B_Pin GPIO_PIN_4
#define ADC_SEL_B_GPIO_Port GPIOA
#define FLASH_SCK_Pin GPIO_PIN_5
#define FLASH_SCK_GPIO_Port GPIOA
#define FLASH_MISO_Pin GPIO_PIN_6
#define FLASH_MISO_GPIO_Port GPIOA
#define FLASH_MOSI_Pin GPIO_PIN_7
#define FLASH_MOSI_GPIO_Port GPIOA
#define FLASH_CS_Pin GPIO_PIN_4
#define FLASH_CS_GPIO_Port GPIOC
#define ADC_SEL_C_Pin GPIO_PIN_0
#define ADC_SEL_C_GPIO_Port GPIOB
#define FCB_UART_TX_Pin GPIO_PIN_10
#define FCB_UART_TX_GPIO_Port GPIOB
#define FCB_UART_RX_Pin GPIO_PIN_11
#define FCB_UART_RX_GPIO_Port GPIOB
#define AUX_I2C_SDA_Pin GPIO_PIN_8
#define AUX_I2C_SDA_GPIO_Port GPIOA
#define AUX_I2C_SCL_Pin GPIO_PIN_9
#define AUX_I2C_SCL_GPIO_Port GPIOA
#define SOL_4_Pin GPIO_PIN_4
#define SOL_4_GPIO_Port GPIOB
#define SOL_5_Pin GPIO_PIN_5
#define SOL_5_GPIO_Port GPIOB
#define SOL_6_Pin GPIO_PIN_6
#define SOL_6_GPIO_Port GPIOB
#define SOL_7_Pin GPIO_PIN_7
#define SOL_7_GPIO_Port GPIOB
#define SOL_8_Pin GPIO_PIN_9
#define SOL_8_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

#define IWDG_HANDLE &hiwdg

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
