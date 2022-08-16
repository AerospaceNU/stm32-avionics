/*
 * board_config.c
 */

#include "board_config_common.h"  // NOLINT

/* Accel */

#if HAS_DEV(ACCEL)
const Orientation_t accelBoardToLocal[NUM_ACCEL][3] = {
    {{AXIS_X, -1}, {AXIS_Y, 1}, {AXIS_Z, -1}},
};
int accelFilterPriority[NUM_ACCEL] = {1};
#endif  // HAS_DEV(ACCEL)

/* Flash */

const uint32_t kFlashSizeBytes[NUM_FLASH] = {0x4000000};

/* IMU */

#if HAS_DEV(IMU)
const Orientation_t imuBoardToLocal[NUM_IMU][3] = {
    {{AXIS_Y, 1}, {AXIS_X, -1}, {AXIS_Z, -1}},
    {{AXIS_Y, 1}, {AXIS_X, -1}, {AXIS_Z, -1}},
};
int imuAccelFilterPriority[NUM_IMU] = {2, 2};
#endif  // HAS_DEV(IMU)

/* Radio */

#if HAS_DEV(RADIO_DESKTOP_SOCKET)
int radioDesktopSocketPort[NUM_RADIO_DESKTOP_SOCKET] = {8080};
#endif  // HAS_DEV(RADIO_DESKTOP_SOCKET)
