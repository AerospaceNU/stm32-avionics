/*
 * board_config.c
 */

#include "board_config_common.h"  // NOLINT

/* Accel */

#if HAS_DEV(ACCEL)
const Orientation_s accelBoardToLocal[NUM_ACCEL][3] = {
    {{AXIS_X, -1}, {AXIS_Y, 1}, {AXIS_Z, -1}},
};
int accelFilterPriority[NUM_ACCEL] = {1};
#endif  // HAS_DEV(ACCEL)

/* Flash */

const uint32_t kFlashSizeBytes[NUM_FLASH] = {0x4000000};

/* IMU */

const Orientation_s imuBoardToLocal[NUM_IMU][3] = {
    {{AXIS_Y, 1}, {AXIS_X, -1}, {AXIS_Z, -1}},
    {{AXIS_Y, 1}, {AXIS_X, -1}, {AXIS_Z, -1}},
};
int imuAccelFilterPriority[NUM_IMU] = {2, 2};

// TODO this is wrong!
const Orientation_s magBoardToLocal[NUM_MAG][3] = {
    {{AXIS_X, 1}, {AXIS_Y, 1}, {AXIS_Z, -1}},
    {{AXIS_X, 1}, {AXIS_Y, 1}, {AXIS_Z, -1}}};

/* Radio */

#if HAS_DEV(RADIO_DESKTOP_SOCKET)
int radioDesktopSocketPort[NUM_RADIO_DESKTOP_SOCKET] = {8080};
#endif  // HAS_DEV(RADIO_DESKTOP_SOCKET)
