/*
 * board_config file for Flight Control Board V1
 */

#ifndef FCB_V1_CORE_INC_BOARD_CONFIG_H_
#define FCB_V1_CORE_INC_BOARD_CONFIG_H_

#include "board_config_types.h"

#define HAL_HEADER "stm32h7xx.h"
#define USE_STM32_HAL_HEADER
#define USE_ARM_MATH
#define USE_STM_HARDWARE_MANAGER

#define NUM_ACCEL_H3LIS331DL 1

#define NUM_BAROMETER_MS5607 2

#define NUM_BLE_CHIP_NRF 1

#define NUM_BLE_CLIENT_STD 1

#define NUM_BUZZER_PWM 1

#define NUM_DC_MOTOR_PWM 0

#define NUM_FLASH_S25FLX 1

#define NUM_GPS_STD 0
#define NUM_GPS_UBLOX 1

#define NUM_IMU_LSM9DS1 2
#define NUM_IMU_ICM20948 0

#define NUM_LED_DIGITAL 5

#define NUM_LINE_CUTTER_BLE 2

#define NUM_PYRO_DIGITAL 6

#define NUM_RADIO_TI_433 0
#define NUM_RADIO_TI_915 1

#define NUM_SERVO_PWM 4

#define NUM_USB_STD 0
#define USB_IS_COMPOSITE
#define NUM_USB_CDC_COMPOSITE 2

#define NUM_VBAT_INA226 1

#define NUM_STM_HADC 1
#define NUM_PYRO_CONT_HADC 6

#define NUM_WATCHDOG_INTERNAL 1

#define RADIO_TI_TYPE RADIO_TI_TYPE_CC1200

#endif  // FCB_V1_CORE_INC_BOARD_CONFIG_H_
