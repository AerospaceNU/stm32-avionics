/*
 * trigger_manager.h
 *
 *  Created on: Apr 4, 2022
 *      Author: sam
 */

#ifndef SYSTEM_TASKS_TRIGGER_MANAGER_H_
#define SYSTEM_TASKS_TRIGGER_MANAGER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "filters.h"

#define MAX_TRIGGER 8

typedef enum {
  TRIGGER_TYPE_PYRO = 1,
  TRIGGER_TYPE_LINE_CUTTER,
  TRIGGER_TYPE_DIGITAL_ON_PYRO,
  TRIGGER_TYPE_DIGITAL_OFF_PYRO
} TriggerType_e;

typedef enum {
  FLAG_APOGEE = 1,
  FLAG_ALT_DURING_DESCENT,
  FLAG_APOGEE_DELAY,
  FLAG_LAUNCH,
  FLAG_TOUCHDOWN,
  FLAG_MANUAL,
  FLAG_CUSTOM_MARMON_CLAMP
} TriggerFlag_e;

typedef struct __attribute__((__packed__)) {
  uint8_t mode;
  uint8_t port;
  uint16_t flags;
  double configValue;
} TriggerConfig_s;

/**
 * @brief Initializes Trigger Manager
 */
void triggerManager_init();

/**
 * @brief Sets apogee timestamp
 * @param timestamp: Timestamp of apogee detection
 */
void triggerManager_setApogeeTime(uint32_t timestamp);

/**
 * @brief Returns which triggers have happened and which have not
 * @return Status of triggers as an 8 bit int where each bit represents if that
 * trigger has happened
 */
uint8_t triggerManager_status();

/**
 * @brief Set the status of triggers based on a 1 byte input
 * @param Status of triggers as an 8 bit int where each bit represents if that
 * trigger has happened
 */
void triggerManager_setTriggerFireStatus(uint8_t status);

/**
 * @brief Update the TriggerManager with new data
 * @param filterData: Current filter data
 * @param hasPassedLaunch: Whether or not we have passed launch
 * @param hasPassedApogee: Whether or not we have passed apogee
 * @param hasPassedTouchdown: Whether or not we have passed touchdown
 */
void triggerManager_update(FilterData_s* filterData, bool hasPassedLaunch,
                           bool hasPassedApogee, bool hasPassedTouchdown);

/**
 * @brief Fire a given trigger
 * @param triggerNum: Trigger number to fire
 * @param logFire: Whether or not we should log the firing. true means we log
 * the fire and the trigger will not fire again during a flight without
 * restarting the board
 */
void triggerManager_triggerFire(uint8_t triggerNum, bool logFire);

#ifdef __cplusplus
}
#endif

#endif  // SYSTEM_TASKS_TRIGGER_MANAGER_H_
