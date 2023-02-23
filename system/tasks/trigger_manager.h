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

#define MAX_TRIGGER 16

typedef enum {
  TRIGGER_TYPE_EMPTY = 0,
  TRIGGER_TYPE_PYRO,
  TRIGGER_TYPE_LINE_CUTTER,
  TRIGGER_TYPE_DIGITAL_ON_PYRO,
  TRIGGER_TYPE_DIGITAL_OFF_PYRO,
  TRIGGER_TYPE_PWM_PYRO
} TriggerType_e;

typedef struct __attribute__((__packed__)) {
  uint8_t mode;
  uint8_t port;
  float duration;
  int pulseWidth;
  uint16_t rootExpressionID;
} TriggerConfig_s;

/**
 * @brief Initializes Trigger Manager
 */
void triggerManager_init();

/**
 * @brief Returns which triggers have happened and which have not
 * @return Status of triggers as an 16 bit int where each bit represents if that
 * trigger has happened
 */
uint16_t triggerManager_status();

/**
 * @brief Set the status of triggers based on a 2 byte input
 * @param Status of triggers as an 16 bit int where each bit represents if that
 * trigger has happened
 */
void triggerManager_setTriggerFireStatus(uint16_t status);

/**
 * @brief Update the TriggerManager with new data
 * @param filterData: Current filter data
 */
void triggerManager_update(FilterData_s *filterData);

/**
 * @brief Fire a given trigger
 * @param triggerNum: Trigger number to fire
 * @param logFire: Whether or not we should log the firing. true means we log
 * the fire and the trigger will not fire again during a flight without
 * restarting the board
 */
void triggerManager_triggerFire(uint8_t triggerNum, bool logFire);

/**
 * @brief Change the configuration for a given trigger.
 * @param triggerNum Trigger number to change
 * @param configString string to parse as config
 */
bool triggerManager_setTriggerConfig(uint8_t triggerNum,
                                     const char **configString);

/**
 * Remove a trigger of a given index.
 * @param triggerNum Trigger number to remove.
 */
void triggerManager_removeTrigger(uint8_t triggerNum);

/**
 * Get a trigger number of a given index.
 * @param triggerNum Trigger number to use.
 * @param Buffer to fill
 * @param size of buffer
 */
void triggerManager_getConfigString(uint8_t triggerNum, char *buffer, int n);
#ifdef __cplusplus
}
#endif

#endif  // SYSTEM_TASKS_TRIGGER_MANAGER_H_
