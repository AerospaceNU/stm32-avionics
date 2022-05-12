/*
 * board_config file for Flight Control Board V0
 *
 */

#ifndef H750VBT6_FLIGHT_BOARD_V0_CORE_INC_BOARD_CONFIG_H_
#define H750VBT6_FLIGHT_BOARD_V0_CORE_INC_BOARD_CONFIG_H_

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
// #define HAS_INA226

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

typedef enum { AXIS_X = 0, AXIS_Y, AXIS_Z } Axis_t;

typedef struct {
  Axis_t axis;
  int8_t direction;
} Orientation_t;

extern Orientation_t IMU1_ACCEL_BOARD_TO_LOCAL[3];
extern Orientation_t IMU2_ACCEL_BOARD_TO_LOCAL[3];
extern Orientation_t HIGH_G_ACCEL_BOARD_TO_LOCAL[3];
extern Orientation_t IMU1_GYRO_BOARD_TO_LOCAL[3];
extern Orientation_t IMU2_GYRO_BOARD_TO_LOCAL[3];
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

#endif  // H750VBT6_FLIGHT_BOARD_V0_CORE_INC_BOARD_CONFIG_H_
