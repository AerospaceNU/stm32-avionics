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

typedef struct __attribute__((__packed__)) {
	float	 gps_lat,			gps_long,			gps_alt;
	double 	 battery_voltage;
	uint8_t  pyro_continuity;
	uint8_t  state;
} TransmitData_t;

void transmitData(SensorData_t* sensorData, FilterData_t* filterData, uint8_t state);

#ifdef __cplusplus
}
#endif

#endif /* DATA_TRANSMISSION_H_ */
