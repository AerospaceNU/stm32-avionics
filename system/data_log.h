/*
 * data_log.h
 */

#ifndef SYSTEM_DATA_LOG_H_
#define SYSTEM_DATA_LOG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "filters.h"
#include "hardware_manager.h"

// Flight metadata shows what data will be logged in what relative location
typedef struct __attribute__((__packed__)) {
  double pressureRef;
  uint8_t launched;
  uint64_t gpsTimestamp;
  uint64_t apogeeTimestamp;
  uint8_t triggerFireStatus;
} FlightMetadata_s;

/**
 * @brief Initializes variables important to the data log
 */
void data_log_init();

/**
 * @brief Erases the entire S25FLx chip
 * @param flashId: ID of flash to erase
 */
void data_log_flash_erase(int flashId);

/**
 * @brief Return the last flight number that was logged
 * @return Last flight number
 */
uint32_t data_log_get_last_flight_num();

/**
 * @brief Return the last flight number that was logged and was launched
 * @return Last launched flight number
 */
uint32_t data_log_get_last_launched_flight_num();

/**
 * @brief Start a new flight in the log
 */
void data_log_assign_flight();

/**
 * @brief Get the metadata from the previous flight and load into the current
 * metadata packet
 */
void data_log_load_last_stored_flight_metadata();

/**
 * @brief Get current flight metadata
 * @return Pointer to current FlightMetadata_s
 */
FlightMetadata_s *data_log_get_flight_metadata();

/**
 * @brief Load metadata from a flight load into the current packet
 */
void data_log_read_flight_num_metadata(uint8_t flightNum);

/**
 * @brief Write metadata to flash
 */
void data_log_write_flight_metadata();

/**
 * @brief Write new packet of data to the data log
 * @param sensorData: Sensor data to write to log
 * @param filterData: Filter info to write to log
 * @param state: Current state ID to write to log
 */
void data_log_write(SensorData_s *sensorData, FilterData_s *filterData,
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
uint32_t data_log_read(uint32_t flightNum, uint32_t maxBytes, uint8_t *pdata,
                       bool reset);

/**
 * @brief Read the timestamp of the last log packet
 * @param flightNum: Flight number to read.
 * @return Last timestamp from given flight
 */
uint32_t data_log_get_last_flight_timestamp(uint32_t flightNum);

/**
 * @brief Get current flash usage relative to total flash size
 * @return integer percentage of flash usage (e.g. 53 == 53% usage)
 */
uint8_t data_log_get_flash_usage();

/**
 * @brief Get most recent stored flash configs and load into cli
 */
void data_log_load_cli_configs();

/**
 * @brief Write current flash configs to metadata sector of flash
 */
void data_log_write_cli_configs();

#ifdef __cplusplus
}
#endif

#endif  // SYSTEM_DATA_LOG_H_