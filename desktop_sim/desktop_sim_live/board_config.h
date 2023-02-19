//
// Created by matth on 3/25/2022.
//

#ifndef DESKTOP_SIM_DESKTOP_SIM_LIVE_BOARD_CONFIG_H_
#define DESKTOP_SIM_DESKTOP_SIM_LIVE_BOARD_CONFIG_H_

#include "board_config_types.h"

#define IS_DESKTOP_SIM 1

#define HAL_HEADER "fake_stm_hal.h"

#define NUM_ACCEL_DESKTOP_FILE 1

#define NUM_BAROMETER_DESKTOP_FILE 2

#define NUM_FLASH_DESKTOP_FILE_BACKED 1

#define NUM_GPS_DESKTOP_FILE 1

#define NUM_IMU_DESKTOP_FILE 2

// TODO add fake bluetooth

#define NUM_PYRO_DESKTOP_PRINT 6

#define NUM_PYRO_CONT_DESKTOP_FILE 6

#define NUM_RADIO_DESKTOP_SOCKET 1

#define NUM_VBAT_DESKTOP_FILE 1

#endif  // DESKTOP_SIM_DESKTOP_SIM_LIVE_BOARD_CONFIG_H_
