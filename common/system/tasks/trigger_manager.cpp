/*
 * trigger_manager.c
 *
 *  Created on: Apr 4, 2022
 *      Author: sam
 */

#include "trigger_manager.h"

#include <limits.h>
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

// TODO num pyro + numlinecutter
static TriggerState triggerConnectivityStatus[NUM_PYRO];

void triggerManager_init() {
  for (int i = 0; i < MAX_TRIGGER; ++i) triggerFireStatus[i] = 0;
  expressionStore.init();

  for (int i = 0; i < NUM_PYRO; i++) {
    triggerConnectivityStatus[i] = (TriggerState::NOT_CONFIGURED);
  }
}

uint16_t triggerManager_status() {
  uint16_t status = 0;
  for (uint16_t i = 0; i < MAX_TRIGGER; ++i) {
    status |= (uint16_t)(triggerFireStatus[i] << i);
  }
  return status;
}

void triggerManager_setDefaultConfig() {
  int i;
  for (i = 0; i < MAX_TRIGGER; ++i) {
    cli_getConfigs()->triggerConfiguration[i].mode = TRIGGER_TYPE_EMPTY;
  }
  for (i = 0; i < MAX_EXPRESSION; ++i) {
    cli_getConfigs()->serializedExprs[i].type = empty;
  }
  expressionStore.init();
}

void triggerManager_setTriggerFireStatus(uint16_t status) {
  for (int i = 0; i < MAX_TRIGGER; ++i) {
    triggerFireStatus[i] = status >> i & 0x1;
  }
}

/**
 * Checks if a trigger(Pyro or linecutter) is configured and if that trigger is
 *connected or not
 *  TODO add linecutter functionality
 *  updates triggerConnectivityStatus array
 **/
void triggerManager_checkExpectedTriggers() {
  for (int i = 0; i < NUM_PYRO; i++) {
    triggerConnectivityStatus[i] = (TriggerState::NOT_CONFIGURED);
  }
  SensorData_s* sensorData = hm_getSensorData();
  int port;
  for (int i = 0; i < MAX_TRIGGER; i++) {
#if HAS_DEV(PYRO_CONT)
    if (cli_getConfigs()->triggerConfiguration[i].mode == TRIGGER_TYPE_PYRO) {
      port = cli_getConfigs()->triggerConfiguration[i].port;
      if (sensorData->pyroContData[port]) {
        triggerConnectivityStatus[port] = (TriggerState::CONNECTED_CONFIGURED);
      } else {
        triggerConnectivityStatus[port] =
            (TriggerState::NOT_CONNECTED_CONFIGURED);
      }
      // TODO else if(cli_getConfigs()->triggerConfiguration[i].mode ==
      // SOMETHING ELSE){
      //}
    }
#endif  // HAS_DEV(PYRO_CONT)
  }
}

void triggerManager_update(FilterData_s* filterData) {
  expressionStore.tick(filterData);
  triggerManager_checkExpectedTriggers();

  for (uint8_t i = 0; i < MAX_TRIGGER; i++) {
    TriggerConfig_s* triggerConfig = i + cli_getConfigs()->triggerConfiguration;
    if (expressionStore.getExprBoolValue(triggerConfig->rootExpressionID) &&
        !triggerFireStatus[i]) {
      triggerManager_triggerFire(i, true);
    }
  }
}

bool triggerManager_setTriggerConfig(uint8_t triggerNum,
                                     const char** configString) {
  uint16_t resultID;
  StringSlice slice =
      StringSlice(configString, 0, strnlen(*configString, INT16_MAX));
  expressionStore.removeExpressionsForTrigger(triggerNum);
  ExpressionValueType_e valueType = expressionStore.parseForTrigger(
      &resultID, triggerNum, slice, expressionStore.getNextExpressionSpot(0));
  if (valueType == ExpressionValueType_e::boolean) {
    expressionStore.writeNewConfigs();
    TriggerConfig_s* triggerConfig =
        triggerNum + cli_getConfigs()->triggerConfiguration;
    triggerConfig->rootExpressionID = resultID;
    return true;
  }
  return false;
}

const TriggerState triggerManager_getExpectedTriggers(uint8_t i) {
  if (i < NUM_PYRO) {
    return triggerConnectivityStatus[i];
  }
  return TriggerState::NOT_CONFIGURED;
}

/**
 * Manage array bc Sam says users are stupid
 */
const char* triggerManager_triggerStatusToString(TriggerState i) {
  const char* triggerStatusToString[] = {"CONNECTED", "NOT_CONNECTED",
                                         "NOT_CONFIGURED"};
  if (static_cast<uint8_t>(i) <
      (sizeof(triggerStatusToString) / sizeof(triggerStatusToString[0]))) {
    return triggerStatusToString[static_cast<uint8_t>(i)];
  }
  return "";
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
    if (!(triggerConfig->port < NUM_PYRO)) return;
    hm_pyroFire(triggerConfig->port,
                static_cast<uint32_t>(triggerConfig->duration));

  } else if (triggerConfig->mode == TRIGGER_TYPE_DIGITAL_ON_PYRO) {
    if (!(triggerConfig->port < NUM_PYRO)) return;
    hm_pyroSet(triggerConfig->port, true);

  } else if (triggerConfig->mode == TRIGGER_TYPE_DIGITAL_OFF_PYRO) {
    if (!(triggerConfig->port < NUM_PYRO)) return;
    hm_pyroSet(triggerConfig->port, false);
  } else if (triggerConfig->mode == TRIGGER_TYPE_PWM_PYRO) {
    if (!(triggerConfig->port < NUM_PYRO)) return;
    hm_pyroSetPwm(triggerConfig->port,
                  static_cast<uint32_t>(triggerConfig->duration), 100,
                  triggerConfig->pulseWidth);
  } else if (triggerConfig->mode == TRIGGER_TYPE_LINE_CUTTER) {
    // Line cutter type
    hm_lineCuttersSendCut(triggerConfig->port);
  } else {
    // Invalid type
    return;
  }

  if (logFire) triggerFireStatus[triggerNum] = true;
  dataLog_getFlightMetadata()->triggerFireStatus = ~triggerManager_status();
  dataLog_writeFlightMetadata();
}
