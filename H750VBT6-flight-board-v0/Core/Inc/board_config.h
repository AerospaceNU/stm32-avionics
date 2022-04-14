/*
 * board_config file for Flight Control Board V0
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

#define HAS_LED_1

#define HAS_SPI
#define HAS_ADC
#define HAS_UART

#define HAS_PYRO
#define MAX_PYRO 3

/*
 * IMU Types, of which there are 2
 */

#define IMU_1 IMU_LSM9DS1
#define IMU_2 IMU_LSM9DS1

/*
 * Baro Types: (up to 2 baros)
 */
#define BARO_1 BARO_MS5607_SPI
#define BARO_2 BARO_MS5607_SPI

/*
 * Radio. We assume we can only have 433 or 915, and they're both
 * either cc1200 or cc1120
 */

#define HAS_RADIO_433
// #define HAS_RADIO_915

#define TELEMETRY_RADIO RADIO_433

#define HAS_RADIO_CLI

#endif
