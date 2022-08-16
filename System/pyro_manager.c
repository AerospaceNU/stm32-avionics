/*
 * pyro_manager.c
 *
 *  Created on: Apr 4, 2022
 *      Author: sam
 */

#include "pyro_manager.h"

#include "board_config_common.h"

#if HAS_DEV(PYRO)

#include <math.h>
#include <stdbool.h>

#include "cli.h"
#include "data_log.h"
#include "filters.h"
#include "hardware_manager.h"

static bool pyroFireStatus[NUM_PYRO] = {0};
static uint64_t apogeeTimestamp;

void PyroManager_Init() {
  for (int i = 0; i < NUM_PYRO; ++i) pyroFireStatus[i] = 0;
  apogeeTimestamp = 0;
}

void PyroManager_SetApogeeTime(uint64_t timestamp) {
  apogeeTimestamp = timestamp;
}

uint8_t PyroManager_Status() {
  uint8_t status = 0;
  for (int i = 0; i < fmin(NUM_PYRO, 8); ++i) {
    status |= pyroFireStatus[i] << i;
  }
  return status;
}

void PyroManager_SetPyroFireStatus(uint8_t status) {
  for (int i = 0; i < fmin(NUM_PYRO, 8); ++i) {
    pyroFireStatus[i] = status >> i & 0x1;
  }
}

void PyroManager_Update(FilterData_t* filterData, bool hasPastApogee) {
  // Turns off expired pyros
  HM_PyroUpdate();
  PyroConfig_t* pyroConfiguration = cliGetConfigs()->pyroConfiguration;
  for (int i = 0; i < NUM_PYRO; i++) {
    if (!pyroFireStatus[i]) {
      // Check that we are after apogee
      if (hasPastApogee) {
        // Check if we want to check for descent altitude
        if (pyroConfiguration->flags == FLAG_ALT_DURING_DESCENT) {
          if (filterData->pos_z < pyroConfiguration->configValue) {
            PyroManager_PyroFire(i, 1000, true);
          }
        } else if (pyroConfiguration->flags == FLAG_APOGEE) {
          // Start apogee pyro
          PyroManager_PyroFire(i, 1000, true);
        } else if (pyroConfiguration->flags == FLAG_APOGEE_DELAY) {
          if (HM_Millis() - apogeeTimestamp >
              1000 * pyroConfiguration->configValue) {
            PyroManager_PyroFire(i, 1000, true);
          }
        }
      }
    }
    ++pyroConfiguration;
  }
}

void PyroManager_PyroFire(uint8_t pyroNum, uint32_t fireTime, bool logFire) {
  if (!(pyroNum >= 0 && pyroNum <= NUM_PYRO)) return;
  HM_PyroFire(pyroNum, fireTime);
  if (logFire) pyroFireStatus[pyroNum] = true;
  data_log_get_flight_metadata()->pyroFireStatus = ~PyroManager_Status();
  data_log_write_flight_metadata();
}

#else

void PyroManager_Init() {}
void PyroManager_SetApogeeTime(uint64_t timestamp) {}
uint8_t PyroManager_Status() { return 255; }
void PyroManager_SetPyroFireStatus(uint8_t status) {}
void PyroManager_Update(FilterData_t* filterData, bool hasPastApogee) {}
void PyroManager_PyroFire(uint8_t pyroNum, uint32_t fireTime, bool logFire) {}

#endif  // HAS_DEV(PYRO)
