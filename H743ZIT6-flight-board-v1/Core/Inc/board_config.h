/*
 * board_config file for amazon-stm32 project
 *
 */


#ifndef BOARD_CONFIG
#define BOARD_CONFIG

#include "stm32h7xx.h"

#define HAS_CC1120
#define HAS_ADC_DEVICE
#define HAS_GPS
#define HAS_MS5607
#define HAS_S25FLX
#define HAS_BUZZER
#define HAS_SERVO
#define HAS_LSM9DS1
#define HAS_H3LIS331DL
#define HAS_DC_MOTOR
#define HAS_USB
#define HAS_INTERNAL_FLASH

#define HAS_LED_1

#define HAS_SPI
#define HAS_ADC
#define HAS_UART

/*
 * IMU Types:
 * 0: NO IMU
 * 1: LSM9DS1
 */

#define IMU_1 1
#define IMU_2 0


#endif
