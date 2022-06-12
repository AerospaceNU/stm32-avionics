//
// Created by matth on 3/25/2022.
//

#ifndef STM32_AVIONICS_BOARD_CONFIG_H
#define STM32_AVIONICS_BOARD_CONFIG_H

#define TELEMETRY_RADIO RADIO_433
#define HAS_PYRO
#define MAX_PYRO 2

#define HAL_HEADER "fake_hal.h"

typedef enum { AXIS_X = 0, AXIS_Y, AXIS_Z } Axis_t;

#include "stdint.h"
typedef struct {
    Axis_t axis;
    int8_t direction;
} Orientation_t;

extern Orientation_t IMU1_ACCEL_BOARD_TO_LOCAL[3];
extern Orientation_t IMU2_ACCEL_BOARD_TO_LOCAL[3];
extern Orientation_t HIGH_G_ACCEL_BOARD_TO_LOCAL[3];
extern Orientation_t IMU1_GYRO_BOARD_TO_LOCAL[3];
extern Orientation_t IMU2_GYRO_BOARD_TO_LOCAL[3];

#include <stdint.h>
void HAL_Delay(uint32_t Delay);

#endif //STM32_AVIONICS_BOARD_CONFIG_H
