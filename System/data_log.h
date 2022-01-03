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

/**
 * @brief Return the last flight number that was logged
 * @return Last flight number
 */
uint32_t data_log_get_last_flight_num();

/**
 * @brief Start a new flight in the log
 */
void data_log_assign_flight();

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

#ifdef __cplusplus
}
#endif


#endif /* DATA_LOG_H_ */
