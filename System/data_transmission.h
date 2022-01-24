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

#define PACKED_STRUCT typedef struct __attribute__((__packed__))

#define MAX_STRING 48 // The maximum length one string sent over radio can be. Longer must be segmented

// Prop data, from the link budget Google sheet
#define TELEMETRY_ID_PROPSTUFF 1
PACKED_STRUCT {
	float loxTankDucer;
	float kerTankDucer;
	float purgeDucer;
	// etc
} PropStuff_t;

// Location data?? from the link budget Google sheet
#define TELEMETRY_ID_POSITION 2
PACKED_STRUCT {
	float temp, pressure, pos_z, vel_z, lat, lon, gps_alt;
	uint32_t gpsTime;
	uint8_t sats, pyro_cont, batt_volts, state, btClients;
	// etc
} PositionData_t;

// Uplinked string (not necessarily null-terminated)
#define TELEMETRY_ID_STRING 3
PACKED_STRUCT {
	char string[MAX_STRING]
	// etc
} CliString_t;

typedef union {
	PropStuff_t propStuff;
	PositionData_t positionData;
	CliString_t cliString;
} PayloadPacket_u;

PACKED_STRUCT {
	uint8_t packetType;
	uint8_t softwareVersion;
	uint32_t timestampMs;
	char callsign[8];
	PayloadPacket_u payload;
} Packet_t;

#define RADIO_PACKET_SIZE sizeof(Packet_t)

void Telemetry_DecodePacket(const Packet_t *packet);

void transmitData(SensorData_t* sensorData, FilterData_t* filterData, uint8_t state);

#ifdef __cplusplus
}
#endif

#endif /* DATA_TRANSMISSION_H_ */
