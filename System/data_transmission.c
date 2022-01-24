/*
 * data_transmission.c
 */

#include "data_transmission.h"
#include "stdio.h"

static Packet_t transmitPacket;

void Telemetry_DecodePacket(const Packet_t *packet) {
	switch (packet->packetType) {
		case TELEMETRY_ID_POSITION: {
			PositionData_t data = packet->payload.positionData;
			printf("lat %f lon %f alt %f sats %u\n", data.lat,
				data.lon, data.gps_alt, data.sats);
			break;
		}
		case TELEMETRY_ID_PROPSTUFF: {
			PropStuff_t data = packet->payload.propStuff;
			printf("loxTank %f kerTank %f purge %f\n",
				data.loxTankDucer, data.kerTankDucer, data.purgeDucer);
			break;
		}
		case TELEMETRY_ID_STRING: {
			CliString_t data = packet->payload.cliString;
			printf("%s", data.string);
			break;
		}
	}
}

void transmitData(SensorData_t* sensorData, FilterData_t* filterData, uint8_t state) {

	// TODO add data to the packet

//	// Gather packet
//	transmitPacket.gps_lat = sensorData->gps_lat;
//	transmitPacket.gps_long = sensorData->gps_long;
//	transmitPacket.gps_alt = sensorData->gps_alt;
//	transmitPacket.baro_pres = (sensorData->baro1_pres + sensorData->baro1_pres) / 2;
//	transmitPacket.battery_voltage = sensorData->battery_voltage;
//	transmitPacket.pyro_continuity = 0;
//	for (int i = 0; i < sizeof(sensorData->pyro_continuity); i++) {
//		transmitPacket.pyro_continuity |= ((sensorData->pyro_continuity[i] & 0x01) << i);
//	}
//	transmitPacket.state = state;

	//TODO redesign this to support the RX check requirement of the radio, see cc1120_state function in CC1120.c
	// Send packet. A lost packet is not important to send again, so don't worry about success
	HM_RadioSend((uint8_t*) &transmitPacket, RADIO_PACKET_SIZE);
}
