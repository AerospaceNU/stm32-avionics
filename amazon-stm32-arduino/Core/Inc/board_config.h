/*
 * board_config file for amazon-stm32 project
 *
 */

#ifndef AMAZON_STM32_ARDUINO_CORE_INC_BOARD_CONFIG_H_
#define AMAZON_STM32_ARDUINO_CORE_INC_BOARD_CONFIG_H_

#define HAL_HEADER "stm32f4xx_hal.h"

#define FCB_VERSION 0

#define HAS_CC1120
//#define HAS_ADC_DEVICE
//#define HAS_GPS
//#define HAS_MS5607
//#define HAS_S25FLX
//#define HAS_BUZZER
//#define HAS_SERVO
//#define HAS_LSM9DS1
//#define HAS_H3LIS331DL
//#define HAS_DC_MOTOR
//#define HAS_USB
//#define HAS_INTERNAL_FLASH

// define HAS_LED_1
// define LED1_Pin 0
#define HAS_SPI
//#define HAS_ADC
//#define HAS_UART
#define NUM_RADIO_TI_433 1
#define NUM_RADIO_TI_915 0
#define RADIO_TI_TYPE RADIO_TI_TYPE_CC1120
#define NUM_USB_STD 1

/*
 * IMU Types:
 * 0: NO IMU
 * 1: LSM9DS1
 */

//#define IMU_1 1
//#define IMU_2 1

#endif  // AMAZON_STM32_ARDUINO_CORE_INC_BOARD_CONFIG_H_
