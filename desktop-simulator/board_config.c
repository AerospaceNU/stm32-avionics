#include "board_config.h"

Orientation_t IMU1_ACCEL_BOARD_TO_LOCAL[3] = {
        {AXIS_X, -1}, {AXIS_Z, -1}, {AXIS_Y, 1}};

Orientation_t IMU2_ACCEL_BOARD_TO_LOCAL[3] = {
        {AXIS_X, -1}, {AXIS_Z, -1}, {AXIS_Y, 1}};

Orientation_t HIGH_G_ACCEL_BOARD_TO_LOCAL[3] = {
        {AXIS_Y, 1}, {AXIS_Z, -1}, {AXIS_X, -1}};

void HAL_Delay(uint32_t ms) {
    auto start = HM_Millis();
    while((HM_Millis() - start) < ms);
}