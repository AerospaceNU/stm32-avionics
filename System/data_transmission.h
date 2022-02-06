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

	uint8_t packetType;
	uint8_t softwareVersion;
	uint32_t timestampMs;
	char callsign[8];

	float	 gps_lat,			gps_long,			gps_alt;
	float pos_z, vel_z;
	float 	 baro_pres;
	double 	 battery_voltage;
	uint8_t  pyro_continuity;
	uint8_t  state;
} TransmitData_t;

void transmitData(SensorData_t* sensorData, FilterData_t* filterData, uint8_t state);

#ifdef __cplusplus
}
#endif

#endif /* DATA_TRANSMISSION_H_ */
