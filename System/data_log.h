/*
 * data_log.h
 */

#ifndef DATA_LOG_H_
#define DATA_LOG_H_

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>
#include "hardware_manager.h"
#include "filters.h"

// Flight metadata shows what data will be logged in what relative location
typedef struct __attribute__((__packed__)) FlightMetadata {
	double pressureRef;
	uint8_t launched;
	uint64_t timestamp;
} FlightMetadata;

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
 * @brief Get the metadata from the previous flight and load into the current metadata packet
 */
void data_log_load_last_stored_flight_metadata();

void data_log_get_flight_metadata(FlightMetadata *metadata, uint8_t flightNum);

FlightMetadata data_log_get_metadata();

/**
 * @brief Start a new flight in the log
 */
void data_log_assign_flight();

/**
 * @brief Set the metadata stored pressure
 */
void data_log_set_pressure_metadata(double presRef);

/**
 * @brief Set the metadata launched field to true
 */
void data_log_set_launched_metadata();

/**
 * @brief Set the metadata timestamp to current timestamp
 */
void data_log_set_timestamp_metadata(uint64_t timestamp);


/**
 * @brief Write metadata to flash
 */
void data_log_write_metadata();

/**
 * @brief Copy an entire metadata packet into current metadata
 */
void data_log_copy_metadata(FlightMetadata *oldMetadataPacket);

/**
 * @brief Write new packet of data to the data log
 * @param sensorData: Sensor data to write to log
 * @param filterData: Filter info to write to log
 * @param state: Current state ID to write to log
 */
void data_log_write(SensorData_t* sensorData, FilterData_t* filterData, uint8_t state);

/**
 * @brief Read data from the log. Flight number is persistent across calls to read data in chunks until reset is True or flightNum changes
 * @param flightNum: Flight number to read. If same as last call, will start reading where last left off
 * @param maxBytes: Maximum number of bytes to read in this single chunk
 * @param pData: Buffer to read data into
 * @param reset: If true, will start reading from the beginning of the specified flightNum no matter what
 * @return Number of bytes actually read from the data log
 */
uint32_t data_log_read(uint32_t flightNum, uint32_t maxBytes, uint8_t *pdata, bool reset);

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

#ifdef __cplusplus
}
#endif


#endif /* DATA_LOG_H_ */
