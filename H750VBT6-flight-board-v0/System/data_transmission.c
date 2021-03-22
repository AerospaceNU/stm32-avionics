/*
 * data_transmission.c
 */

#include "data_transmission.h"

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

	//TODO redesign this to support the RX check requirement of the radio, see cc1120_state function in CC1120.c
	// Send packet. A lost packet is not important to send again, so don't worry about success
	HM_RadioSend((uint8_t*) &transmitPacket, sizeof(transmitPacket));
}
