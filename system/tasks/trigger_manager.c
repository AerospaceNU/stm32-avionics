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
static uint32_t apogeeTimestamp;

void triggerManager_init() {
  for (int i = 0; i < MAX_TRIGGER; ++i) triggerFireStatus[i] = 0;
  apogeeTimestamp = 0;
}

void triggerManager_setApogeeTime(uint32_t timestamp) {
  apogeeTimestamp = timestamp;
}

uint8_t triggerManager_status() {
  uint8_t status = 0;
  for (int i = 0; i < MAX_TRIGGER; ++i) {
    status |= triggerFireStatus[i] << i;
  }
  return status;
}

void triggerManager_setTriggerFireStatus(uint8_t status) {
  for (int i = 0; i < MAX_TRIGGER; ++i) {
    triggerFireStatus[i] = status >> i & 0x1;
  }
}

void triggerManager_update(FilterData_s* filterData, bool hasPassedLaunch,
                           bool hasPassedApogee, bool hasPassedTouchdown) {
  // Turns off expired pyros (this is only a pyro thing, the line cutters
  // shut themselves off
  hm_pyroUpdate();
  for (int i = 0; i < MAX_TRIGGER; i++) {
    TriggerConfig_s* triggerConfig =
        &(cli_getConfigs()->triggerConfiguration[i]);
    if (!triggerFireStatus[i]) {
      if (triggerConfig->flags == FLAG_LAUNCH && hasPassedLaunch) {
        triggerManager_triggerFire(i, true);
      }
      if (triggerConfig->flags == FLAG_TOUCHDOWN && hasPassedTouchdown) {
        triggerManager_triggerFire(i, true);
      }
      // Check if we want to check for descent altitude
      if (triggerConfig->flags == FLAG_ALT_DURING_DESCENT && hasPassedApogee) {
        if (filterData->pos_z_agl < triggerConfig->configValue) {
          triggerManager_triggerFire(i, true);
        }
      } else if (triggerConfig->flags == FLAG_APOGEE && hasPassedApogee) {
        // Start apogee pyro
        triggerManager_triggerFire(i, true);
      } else if (triggerConfig->flags == FLAG_APOGEE_DELAY && hasPassedApogee) {
        if (hm_millis() - apogeeTimestamp > 1000 * triggerConfig->configValue) {
          triggerManager_triggerFire(i, true);
        }
      } else if (triggerConfig->flags == FLAG_CUSTOM_MARMON_CLAMP) {
        if (hasPassedLaunch && filterData->world_vel_z < 18.3 &&
            filterData->world_acc_z < 10) {
          triggerManager_triggerFire(i, true);
        }
      }
    }
  }
}

void triggerManager_triggerFire(uint8_t triggerNum, bool logFire) {
  // Pyro type
  TriggerConfig_s* triggerConfig =
      triggerNum + cli_getConfigs()->triggerConfiguration;
  if (triggerConfig->mode == TRIGGER_TYPE_PYRO) {
    if (!(triggerConfig->port >= 0 && triggerConfig->port < NUM_PYRO)) return;
    hm_pyroFire(triggerConfig->port, 1000);

  } else if (triggerConfig->mode == TRIGGER_TYPE_DIGITAL_ON_PYRO) {
    if (!(triggerConfig->port >= 0 && triggerConfig->port < NUM_PYRO)) return;
    hm_pyroSet(triggerConfig->port, true);

  } else if (triggerConfig->mode == TRIGGER_TYPE_DIGITAL_OFF_PYRO) {
    if (!(triggerConfig->port >= 0 && triggerConfig->port < NUM_PYRO)) return;
    hm_pyroSet(triggerConfig->port, false);

    // Line cutter type
  } else if (triggerConfig->mode == TRIGGER_TYPE_LINE_CUTTER) {
    hm_lineCuttersSendCut(triggerConfig->port);
  } else {
    // Invalid type
    return;
  }

  if (logFire) triggerFireStatus[triggerNum] = true;
  dataLog_getFlightMetadata()->triggerFireStatus = ~triggerManager_status();
  dataLog_writeFlightMetadata();
}
