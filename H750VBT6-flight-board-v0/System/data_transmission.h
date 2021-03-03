/*
 * data_transmission.h
 */

#ifndef DATA_TRANSMISSION_H_
#define DATA_TRANSMISSION_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "hardware_manager.h"
#include "filters.h"

void transmitData(SensorData_t* sensorData, FilterData_t* filterData, uint8_t state);

#ifdef __cplusplus
}
#endif

#endif /* DATA_TRANSMISSION_H_ */
