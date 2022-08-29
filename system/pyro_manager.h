/*
 * pyro_manager.h
 *
 *  Created on: Apr 4, 2022
 *      Author: sam
 */

#ifndef SYSTEM_PYRO_MANAGER_H_
#define SYSTEM_PYRO_MANAGER_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "filters.h"

#define FLAG_APOGEE 1 << 0
#define FLAG_ALT_DURING_DESCENT 1 << 1
#define FLAG_APOGEE_DELAY 1 << 2

typedef struct {
  uint16_t flags;
  double configValue;
} PyroConfig_t;

/**
 * @brief Initializes Pyro Manager
 */
void PyroManager_Init();

/**
 * @brief Sets apogee timestamp
 * @param timestamp: Timestamp of apogee detection
 */
void PyroManager_SetApogeeTime(uint64_t timestamp);

/**
 * @brief Returns which pyros have fired and which have not
 * @return Status of pyros as an 8 bit int where each bit represents if that
 * pyro has fired
 */
uint8_t PyroManager_Status();

/**
 * @brief Set the fire status of pyros based on a 1 byte input
 * @param Status of pyros as an 8 bit int where each bit represents if that pyro
 * has fired
 */
void PyroManager_SetPyroFireStatus(uint8_t status);

/**
 * @brief Update the PyroManager with new data
 * @param filterData: Current filter data
 * @param hasPastApogee: Whether or not we have passed apogee
 */
void PyroManager_Update(FilterData_t *filterData, bool hasPastApogee);

/**
 * @brief Fire a given pyro
 * @param pyroNum: Pyro number to fire
 * @param fireTime: Time to run current through pyro in ms
 * @param logFire: Whether or not we should log the firing. true means we log
 * the fire and the pyro will not fire again during a flight without restarting
 * the board
 */
void PyroManager_PyroFire(uint8_t pyroNum, uint32_t fireTime, bool logFire);

#ifdef __cplusplus
}
#endif
#endif /* SYSTEM_PYRO_MANAGER_H_ */
