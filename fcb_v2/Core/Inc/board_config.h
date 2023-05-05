/*
 * board_config file for Flight Control Board V2
 */

#ifndef FCB_V2_CORE_INC_BOARD_CONFIG_H_
#define FCB_V2_CORE_INC_BOARD_CONFIG_H_

#include "board_config_types.h"

#define HAL_HEADER "stm32l4xx.h"

#define NUM_ACCEL_H3LIS331DL 0

#define NUM_BAROMETER_MS5607 2

#define NUM_BLE_CHIP_NRF 1

#define NUM_BLE_CLIENT_STD 1

#define NUM_BUZZER_PWM 1

#define NUM_DC_MOTOR_PWM 0

#define NUM_FLASH_S25FLX 1
#define NUM_FLASH_MB85RSX 1

#define NUM_GPS_STD 0
#define NUM_GPS_UBLOX 1

#define NUM_IMU_ICM20600 0
#define NUM_MAG_IIS2MDC 0
#define NUM_IMU_ICM20948 1

#define NUM_LED_DIGITAL 1

#define NUM_LINE_CUTTER_BLE 2

#define NUM_PYRO_DIGITAL 5

// Everything is attached to adc1, so only one HADC
#define NUM_STM_HADC 1

// Number of pyro and battery analog inputs total
#define NUM_PYRO_CONT_HADC 4
#define NUM_VBAT_ADC 2

#define NUM_RADIO_TI_433 0
#define NUM_RADIO_TI_915 0

#define NUM_SERVO_PWM 4

#define NUM_USB_STD 1


#define NUM_WATCHDOG_INTERNAL 1

#define RADIO_CLI_ID 0
#define USB_CLI_ID 0

#define RADIO_TI_TYPE RADIO_TI_TYPE_CC1200

#endif  // FCB_V2_CORE_INC_BOARD_CONFIG_H_
