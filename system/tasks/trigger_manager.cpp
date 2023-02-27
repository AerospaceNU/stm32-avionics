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
#include "expression_store.h"
#include "filters.h"
#include "hardware_manager.h"
#include "string_slice.h"

static ExpressionStore expressionStore;
static bool triggerFireStatus[MAX_TRIGGER] = {0};

void triggerManager_init() {
  for (int i = 0; i < MAX_TRIGGER; ++i) triggerFireStatus[i] = 0;
  expressionStore = ExpressionStore();
}

uint16_t triggerManager_status() {
  uint16_t status = 0;
  for (int i = 0; i < MAX_TRIGGER; ++i) {
    status |= triggerFireStatus[i] << i;
  }
  return status;
}

void triggerManager_setTriggerFireStatus(uint16_t status) {
  for (int i = 0; i < MAX_TRIGGER; ++i) {
    triggerFireStatus[i] = status >> i & 0x1;
  }
}

void triggerManager_update(FilterData_s* filterData) {
  expressionStore.tick(filterData);

  for (int i = 0; i < MAX_TRIGGER; i++) {
    TriggerConfig_s* triggerConfig = i + cli_getConfigs()->triggerConfiguration;
    if (expressionStore.getStatusFor(triggerConfig->rootExpressionID) &&
        !triggerFireStatus[i]) {
      triggerManager_triggerFire(i, true);
    }
  }
}

bool triggerManager_setTriggerConfig(uint8_t triggerNum,
                                     const char** configString) {
  int resultID;
  StringSlice slice = StringSlice(configString, 0, strlen(*configString));
  expressionStore.removeExpressionsForTrigger(triggerNum);
  ExpressionValueType_e valueType = expressionStore.parseForTrigger(
      &resultID, triggerNum, slice, expressionStore.getNextExpressionSpot(0));
  if (valueType == ExpressionValueType_e::boolean) {
    expressionStore.writeNewConfigs();
    return true;
  }
  return false;
}

void triggerManager_removeTrigger(uint8_t triggerNum) {
  expressionStore.removeExpressionsForTrigger(triggerNum);
  expressionStore.writeNewConfigs();
}

void triggerManager_getConfigString(uint8_t triggerNum, char* buffer, int n) {
  TriggerConfig_s* triggerConfig =
      triggerNum + cli_getConfigs()->triggerConfiguration;
  expressionStore.conditionToString(triggerConfig->rootExpressionID, buffer, n);
}

void triggerManager_triggerFire(uint8_t triggerNum, bool logFire) {
  // Pyro type
  TriggerConfig_s* triggerConfig =
      triggerNum + cli_getConfigs()->triggerConfiguration;
  if (triggerConfig->mode == TRIGGER_TYPE_PYRO) {
    if (!(triggerConfig->port >= 0 && triggerConfig->port < NUM_PYRO)) return;
    hm_pyroFire(triggerConfig->port, triggerConfig->duration);

  } else if (triggerConfig->mode == TRIGGER_TYPE_DIGITAL_ON_PYRO) {
    if (!(triggerConfig->port >= 0 && triggerConfig->port < NUM_PYRO)) return;
    hm_pyroSet(triggerConfig->port, true);

  } else if (triggerConfig->mode == TRIGGER_TYPE_DIGITAL_OFF_PYRO) {
    if (!(triggerConfig->port >= 0 && triggerConfig->port < NUM_PYRO)) return;
    hm_pyroSet(triggerConfig->port, false);
  } else if (triggerConfig->mode == TRIGGER_TYPE_PWM_PYRO) {
    if (!(triggerConfig->port >= 0 && triggerConfig->port < NUM_PYRO)) return;
    hm_pyroSetPwm(triggerConfig->port, triggerConfig->duration, 100,
                  triggerConfig->pulseWidth);
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
