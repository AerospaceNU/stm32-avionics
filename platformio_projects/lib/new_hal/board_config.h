//
// Created by nathan on 5/13/23.
//

#ifndef PLATFORMIO_PROJECTS_LIB_NEW_HAL_BOARD_CONFIG_H_
#define PLATFORMIO_PROJECTS_LIB_NEW_HAL_BOARD_CONFIG_H_

#include "board_config_types.h"

// Fix some STM32 HAL stuff that's still around
// TODO: I really don't know what this does
#define ADC_HandleTypeDef int

// Turn on various features
#define NUM_OTHER_RADIO 1
#define NUM_OTHER_IMU 1
#define NUM_OTHER_MAG 1
#define NUM_OTHER_BAROMETER 1
#define NUM_OTHER_GPS 1
#define NUM_OTHER_ACCEL 0
#define NUM_OTHER_VBAT_ADC 1
#define NUM_USB_STD 1
#define NUM_OTHER_LED 1

// Limit the amount of data we cache in RAM
#define FLASH_MAX_SECTOR_BYTES 1000

#endif  // PLATFORMIO_PROJECTS_LIB_NEW_HAL_BOARD_CONFIG_H_
