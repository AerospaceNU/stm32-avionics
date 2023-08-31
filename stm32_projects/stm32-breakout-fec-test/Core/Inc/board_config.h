/*
 * board_config file for amazon-stm32 project
 *
 */

#ifndef BOARD_CONFIG
#define BOARD_CONFIG

#define HAL_HEADER "stm32f4xx_hal.h"
#define USE_STM32_HAL_HEADER
#define USE_STM_HARDWARE_MANAGER
#define FCB_VERSION 0

#define HAS_CC1120
//#define HAS_ADC_DEVICE
#define NUM_GPS_STD 1
#define NUM_BAROMETER_MS5607 1
//#define HAS_S25FLX
//#define HAS_BUZZER
//#define HAS_SERVO
//#define HAS_LSM9DS1
//#define HAS_H3LIS331DL
//#define HAS_DC_MOTOR
//#define HAS_USB
//#define HAS_INTERNAL_FLASH
#define NUM_USB_STD 1

// define HAS_LED_1
// define LED1_Pin 0
#define HAS_SPI
//#define HAS_ADC
#define HAS_UART

/*
 * IMU Types:
 * 0: NO IMU
 * 1: LSM9DS1
 */

//#define IMU_1 1
//#define IMU_2 1

#endif
