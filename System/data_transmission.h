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
#include <stdint.h>

#define PACKET typedef struct __attribute__((__packed__))

// Prop data, from the link budget Google sheet
PACKET {
	float loxTankDucer;
	float kerTankDucer;
	float purgeDucer;
	// etc
} PropStuff_t;

// Location data?? from the link budget Google sheet
PACKET {
	float temp, pressure, pos_z, vel_z, lat, lon, gps_alt;
	uint32_t gpsTime;
	uint8_t sats, pyro_cont, batt_volts, state, btClients;
	// etc
} PositionData_t;

typedef union {
	PropStuff_t propStuff;
	PositionData_t positionData;
} PayloadPacket_u;

typedef struct __attribute__((__packed__)) {
	uint8_t packetType;
	uint32_t timestampMs;
	char callsign[8];
	PacketPayload_u payload;
} Packet_t;

#define RADIO_PACKET_SIZE sizeof(Packet_t);

void transmitData(SensorData_t* sensorData, FilterData_t* filterData, uint8_t state);

#ifdef __cplusplus
}
#endif

#endif /* DATA_TRANSMISSION_H_ */
