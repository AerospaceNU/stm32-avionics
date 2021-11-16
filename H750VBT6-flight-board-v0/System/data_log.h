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
} FlightMetadata;

uint32_t data_log_get_last_flight_num();

FlightMetadata data_log_get_last_stored_flight_metadata();

void data_log_assign_flight();

void data_log_write_pressure_metadata();

void data_log_write(SensorData_t* sensorData, FilterData_t* filterData, uint8_t state);

uint32_t data_log_read(uint32_t flightNum, uint32_t maxBytes, uint8_t *pdata);

#ifdef __cplusplus
}
#endif


#endif /* DATA_LOG_H_ */
