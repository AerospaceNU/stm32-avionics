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

uint32_t data_log_get_last_flight_num();

void data_log_assign_flight();

void data_log_write(SensorData_t* sensorData, uint8_t state);

uint32_t data_log_read(uint32_t flightNum, uint32_t maxBytes, uint8_t *pdata);

#ifdef __cplusplus
}
#endif


#endif /* DATA_LOG_H_ */
