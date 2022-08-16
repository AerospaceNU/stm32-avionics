/*
 * board_config for groundstation v1
 */

#ifndef GROUNDSTATION_CORE_INC_BOARD_CONFIG_H_
#define GROUNDSTATION_CORE_INC_BOARD_CONFIG_H_

#include "board_config_types.h"

#define HAL_HEADER "stm32h7xx.h"

#define NUM_ACCEL_H3LIS331DL 0

#define NUM_BAROMETER_MS5607 1

#define NUM_BLE_CHIP_NRF 0

#define NUM_BLE_CLIENT_STD 0

#define NUM_BUZZER_PWM 0

#define NUM_DC_MOTOR_PWM 0

#define NUM_FLASH_S25FLX 0

#define NUM_GPS_STD 0
#define NUM_GPS_UBLOX 1

#define NUM_IMU_LSM9DS1 0

#define NUM_LED_DIGITAL 0

#define NUM_LINE_CUTTER_BLE 0

#define NUM_PYRO_DIGITAL 0

#define NUM_PYRO_CONT_ADC 0

#define NUM_RADIO_TI_433 1
#define NUM_RADIO_TI_915 1

#define NUM_SERVO_PWM 0

#define NUM_USB_STD 1

#define NUM_VBAT_ADC 0
#define NUM_VBAT_INA226 0

#define NUM_WATCHDOG_INTERNAL 1

#define BLE_CLI_ID 0
#define RADIO_CLI_ID 0
#define USB_CLI_ID 0

#define RADIO_TI_TYPE RADIO_TI_TYPE_CC1200

#endif  // GROUNDSTATION_CORE_INC_BOARD_CONFIG_H_
