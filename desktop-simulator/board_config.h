//
// Created by matth on 3/25/2022.
//

#ifndef DESKTOP_SIMULATOR_BOARD_CONFIG_H_
#define DESKTOP_SIMULATOR_BOARD_CONFIG_H_

#include <stdint.h>

#define TELEMETRY_RADIO RADIO_433
#define HAS_PYRO
#define MAX_PYRO 2

#define HAL_HEADER "fake_stm_hal.h"
#define IS_DESKTOP

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

void HAL_Delay(uint32_t Delay);

#endif  // DESKTOP_SIMULATOR_BOARD_CONFIG_H_
