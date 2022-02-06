/*
 * board_config file for V0
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
#define IMU_2 1

/*
 * Baro Types: (up to 2 baros)
 * 0: No Baro
 * 1: MS5607 over SPI
 */
#define BARO_1 1
#define BARO_2 1

/*
 * Radio. We assume we can only have 433 or 915, and they're both
 * either cc1200 or cc1120
 */

#define HAS_RADIO_433
// #define HAS_RADIO_915

#endif
