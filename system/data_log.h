/*
 * data_log.h
 */

#ifndef SYSTEM_DATA_LOG_H_
#define SYSTEM_DATA_LOG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "cli.h"
#include "filters.h"
#include "hardware_manager.h"

// Flight metadata shows what data will be logged in what relative location
typedef struct __attribute__((__packed__)) {
  double pressureRef;
  int8_t gravityRef;
  uint8_t launched;
  uint64_t gpsTimestamp;
  uint32_t apogeeTimestamp;
  uint8_t triggerFireStatus;
  CliConfigs_s launchedCliConfigs;
} FlightMetadata_s;

/**
 * @brief Initializes variables important to the data log
 */
void dataLog_init();

/**
 * @brief Erases an entire flash chip within the data log
 * @param flashId: ID of flash to erase
 */
void dataLog_flashErase(int flashId);

/**
 * @brief Return the last flight number that was logged
 * @return Last flight number
 */
uint32_t dataLog_getLastFlightNum();

/**
 * @brief Return the last flight number that was logged and was launched
 * @return Last launched flight number
 */
uint32_t dataLog_getLastLaunchedFlightNum();

/**
 * @brief Start a new flight in the log
 */
void dataLog_assignFlight();

/**
 * @brief Get the metadata from the previous flight and load into the current
 * metadata packet
 */
void dataLog_loadLastStoredFlightMetadata();

/**
 * @brief Get current flight metadata
 * @return Pointer to current FlightMetadata_s
 */
FlightMetadata_s *dataLog_getFlightMetadata();

/**
 * @brief Load metadata from a flight load into the current packet
 */
void dataLog_readFlightNumMetadata(uint8_t flightNum);

/**
 * @brief Write metadata to flash
 */
void dataLog_writeFlightMetadata();

/**
 * @brief Write new packet of data to the data log
 * @param sensorData: Sensor data to write to log
 * @param filterData: Filter info to write to log
 * @param state: Current state ID to write to log
 */
void dataLog_write(SensorData_s *sensorData, FilterData_s *filterData,
                   uint8_t state);

/**
 * @brief Read data from the log. Flight number is persistent across calls to
 * read data in chunks until reset is True or flightNum changes
 * @param flightNum: Flight number to read. If same as last call, will start
 * reading where last left off
 * @param maxBytes: Maximum number of bytes to read in this single chunk
 * @param pData: Buffer to read data into
 * @param reset: If true, will start reading from the beginning of the specified
 * flightNum no matter what
 * @return Number of bytes actually read from the data log
 */
uint32_t dataLog_read(uint32_t flightNum, uint32_t maxBytes, uint8_t *pdata,
                      bool reset);

/**
 * @brief Read the timestamp of the last log packet
 * @param flightNum: Flight number to read.
 * @return Last timestamp from given flight
 */
uint32_t dataLog_getLastFlightTimestamp(uint32_t flightNum);

/**
 * @brief Get current flash usage relative to total flash size
 * @return integer percentage of flash usage (e.g. 53 == 53% usage)
 */
uint8_t dataLog_getFlashUsage();

/**
 * @brief Get most recent stored flash configs and load into cli
 */
void dataLog_loadCliConfigs();

/**
 * @brief Write current flash configs to metadata sector of flash
 */
void dataLog_writeCliConfigs();

#ifdef __cplusplus
}
#endif

#endif  // SYSTEM_DATA_LOG_H_
