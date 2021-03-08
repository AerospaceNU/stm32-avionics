/*
 * data_transmission.c
 */

#include "data_transmission.h"

typedef struct __attribute__((__packed__)) {
	double	 gps_lat,			gps_long,			gps_alt;
	double 	 battery_voltage;
	uint8_t  pyro_continuity;
	uint8_t  state;
} TransmitData_t;

static TransmitData_t transmitPacket;

void transmitData(SensorData_t* sensorData, FilterData_t* filterData, uint8_t state) {

	// Gather packet
	transmitPacket.gps_lat = sensorData->gps_lat;
	transmitPacket.gps_long = sensorData->gps_long;
	transmitPacket.gps_alt = sensorData->gps_alt;
	transmitPacket.battery_voltage = sensorData->battery_voltage;
	transmitPacket.pyro_continuity = 0;
	for (int i = 0; i < sizeof(sensorData->pyro_continuity); i++) {
		transmitPacket.pyro_continuity |= ((sensorData->pyro_continuity[i] & 0x01) << i);
	}
	transmitPacket.state = state;

	// Send packet. A lost packet is not important to send again, so don't worry about success
	HM_RadioSend((uint8_t*) &transmitPacket, sizeof(transmitPacket));
}
