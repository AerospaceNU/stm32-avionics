/*
 * board_config file for Flight Control Board V0
 */

#ifndef STM32_PROJECTS_ROCKETBEACON_CORE_INC_BOARD_CONFIG_H_
#define STM32_PROJECTS_ROCKETBEACON_CORE_INC_BOARD_CONFIG_H_

#include "board_config_types.h"

#define HAL_HEADER "stm32wlxx.h"
#define USE_STM32_HAL_HEADER
#undef USE_ARM_MATH
#define USE_STM_HARDWARE_MANAGER

// I think this is right? idk
#define ARM_MATH_CM4

#define NUM_GPS_STD 0
#define NUM_GPS_UBLOX 1
#define GPS_ENABLE_VTG // enable speed/course over ground

#define NUM_LED_DIGITAL 1

#define NUM_RADIO_STM32WL 1

#define NUM_WATCHDOG_INTERNAL 0

#define RADIO_CLI_ID 0
#define USB_CLI_ID 0

#define RADIO_TI_TYPE RADIO_TI_TYPE_CC1120

// This can be up to +22dbm (limit to 16 on coin cell due to current draw though
#define RADIO_TX_POWER_DBM 10
// channel 0 frequency, mhz
#define RADIO_CENTER_FREQ 915

// huge hack since we don't have SPI
struct SPI_HandleTypeDef;

#endif  // STM32_PROJECTS_ROCKETBEACON_CORE_INC_BOARD_CONFIG_H_
