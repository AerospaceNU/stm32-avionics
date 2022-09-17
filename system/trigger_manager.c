/*
 * trigger_manager.c
 *
 *  Created on: Apr 4, 2022
 *      Author: sam
 */

#include "trigger_manager.h"

#include <math.h>
#include <stdbool.h>

#include "board_config_common.h"
#include "cli.h"
#include "data_log.h"
#include "filters.h"
#include "hardware_manager.h"

static bool triggerFireStatus[MAX_TRIGGER] = {0};
static uint64_t apogeeTimestamp;

void TriggerManager_Init() {
  for (int i = 0; i < MAX_TRIGGER; ++i) triggerFireStatus[i] = 0;
  apogeeTimestamp = 0;
}

void TriggerManager_SetApogeeTime(uint64_t timestamp) {
  apogeeTimestamp = timestamp;
}

uint8_t TriggerManager_Status() {
  uint8_t status = 0;
  for (int i = 0; i < MAX_TRIGGER; ++i) {
    status |= triggerFireStatus[i] << i;
  }
  return status;
}

void TriggerManager_SetTriggerFireStatus(uint8_t status) {
  for (int i = 0; i < MAX_TRIGGER; ++i) {
    triggerFireStatus[i] = status >> i & 0x1;
  }
}

void TriggerManager_Update(FilterData_s* filterData, bool hasPassedLaunch,
                           bool hasPassedApogee, bool hasPassedTouchdown) {
  // Turns off expired pyros (this is only a pyro thing, the line cutters
  // shut themselves off
  HM_PyroUpdate();
  TriggerConfig_s* triggerConfiguration = cliGetConfigs()->triggerConfiguration;
  for (int i = 0; i < MAX_TRIGGER; i++) {
    if (!triggerFireStatus[i]) {
      if (triggerConfiguration->flags == FLAG_LAUNCH && hasPassedLaunch) {
        TriggerManager_TriggerFire(i, true);
      }
      if (triggerConfiguration->flags == FLAG_TOUCHDOWN && hasPassedTouchdown) {
        TriggerManager_TriggerFire(i, true);
      }
      // Check if we want to check for descent altitude
      if (triggerConfiguration->flags == FLAG_ALT_DURING_DESCENT &&
          hasPassedApogee) {
        if (filterData->pos_z < triggerConfiguration->configValue) {
          TriggerManager_TriggerFire(i, true);
        }
      } else if (triggerConfiguration->flags == FLAG_APOGEE &&
                 hasPassedApogee) {
        // Start apogee pyro
        TriggerManager_TriggerFire(i, true);
      } else if (triggerConfiguration->flags == FLAG_APOGEE_DELAY &&
                 hasPassedApogee) {
        if (HM_Millis() - apogeeTimestamp >
            1000 * triggerConfiguration->configValue) {
          TriggerManager_TriggerFire(i, true);
        }
      } else if (triggerConfiguration->flags == FLAG_CUSTOM_MARMON_CLAMP) {
        if (hasPassedLaunch && filterData->world_vel_z < 18.3 &&
            filterData->world_acc_z < 10) {
          TriggerManager_TriggerFire(i, true);
        }
      }
    }
    ++triggerConfiguration;
  }
}

void TriggerManager_TriggerFire(uint8_t triggerNum, bool logFire) {
  // Pyro type
  TriggerConfig_s* triggerConfig =
      triggerNum + cliGetConfigs()->triggerConfiguration;
  if (triggerConfig->mode == TRIGGER_TYPE_PYRO) {
    if (!(triggerConfig->port >= 0 && triggerConfig->port < NUM_PYRO)) return;
    HM_PyroFire(triggerConfig->port, 1000);

  } else if (triggerConfig->mode == TRIGGER_TYPE_DIGITAL_ON_PYRO) {
    if (!(triggerConfig->port >= 0 && triggerConfig->port < NUM_PYRO)) return;
    HM_PyroSet(triggerConfig->port, true);

  } else if (triggerConfig->mode == TRIGGER_TYPE_DIGITAL_OFF_PYRO) {
    if (!(triggerConfig->port >= 0 && triggerConfig->port < NUM_PYRO)) return;
    HM_PyroSet(triggerConfig->port, false);

    // Line cutter type
  } else if (triggerConfig->mode == TRIGGER_TYPE_LINE_CUTTER) {
    HM_LineCuttersSendCut(triggerConfig->port);
  } else {
    // Invalid type
    return;
  }

  if (logFire) triggerFireStatus[triggerNum] = true;
  data_log_get_flight_metadata()->triggerFireStatus = ~TriggerManager_Status();
  data_log_write_flight_metadata();
}
