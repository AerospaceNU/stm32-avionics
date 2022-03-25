/*
 * data_transmission.c
 */

#include "radio_manager.h"
#include "stdio.h"
#include "board_config.h"
#include "string.h"
#include "cli.h"

static DataRecieveState_t dataRx;

static RadioPacket_t transmitPacket;

void RadioManager_init() {
	cbInit(&dataRx.rxBuffer, dataRx.rxArray, RX_BUFF_LEN,
			sizeof(RecievedPacket_t));
	HM_RadioRegisterConsumer(RADIO_433, &dataRx.rxBuffer);
	HM_RadioRegisterConsumer(RADIO_915, &dataRx.rxBuffer);

	const char *call = "KM6GNL";
	strncpy(transmitPacket.callsign, call, 8);

#ifdef SOFTWARE_VERSION
	transmitPacket.softwareVersion = SOFTWARE_VERSION; // TODO set this;
#endif
	transmitPacket.board_serial_num = 0; // TODO set this;

#ifdef TELEMETRY_RADIO
	HM_RadioSetChannel(TELEMETRY_RADIO, 2);
#endif
}

//! Must be called periodically to output data over CLI or USB
void RadioManager_tick() {
	static RecievedPacket_t packet;

	// Try to dequeue all the packet's we've gotten
	static size_t len = sizeof(packet);
	while (cbCount(&dataRx.rxBuffer)) {
		cbPeek(&dataRx.rxBuffer, &packet, &len);
		if (len) {

			// Send to all our callbacks
			for(int i = 0; i < dataRx.numCallbacks; i++) {
				if(dataRx.callbacks[i]) dataRx.callbacks[i](&packet);
			}

			cbDequeue(&dataRx.rxBuffer, 1);
		} else {
			cbFlush(&dataRx.rxBuffer);
		}
	}
}

void RadioManager_addMessageCallback(RadioCallback_t callback) {
	dataRx.callbacks[dataRx.numCallbacks] = callback;
	dataRx.numCallbacks++;
}

// Packet rates, in hz
#define ORIENTATION_RATE 1
#define POSITION_RATE 10

#ifdef TELEMETRY_RADIO
static DataTransmitState_t lastSent;
void RadioManager_transmitData(SensorData_t *sensorData,
		FilterData_t *filterData, uint8_t state) {
	uint32_t currentTime = HM_Millis();
	transmitPacket.timestampMs = currentTime;

	// if (currentTime - lastSent.propStuffLastSent >= 500) {
	// 	PropulsionPacket_t data = {
	// 		sensorData->loxTankDucer,
	// 		sensorData->kerTankDucer,
	// 		sensorData->purgeDucer,
	// 		sensorData->loxInletDucer,
	// 		sensorData->kerInletDucer,
	// 		sensorData->loxVenturi,
	// 		sensorData->kerVenturi,
	// 		sensorData->loadcell,
	// 		sensorData->loxTank,
	// 		sensorData->injector,
	// 		sensorData->engine
	// 	};
	// 	transmitPacket.packetType = 1;
	// 	transmitPacket.payload = data;
	// 	lastSent.propStuffLastSent = 0;
	// } else

	if (currentTime - lastSent.orientationLastSent >= 1000 / ORIENTATION_RATE) {
		OrientationPacket_t data = { state, (filterData->qw * 100.0),
				(filterData->qx * 100.0), filterData->qy * 100.0,
				(filterData->qz * 100.0), sensorData->imu1_gyro_x,
				sensorData->imu1_gyro_y, sensorData->imu1_gyro_z,
				filterData->acc_x, filterData->acc_y, filterData->acc_z,
				sensorData->imu1_mag_x, sensorData->imu1_mag_y,
				sensorData->imu1_mag_z };
		transmitPacket.packetType = 2;
		transmitPacket.payload.orientation = data;
		lastSent.orientationLastSent = currentTime;

		HM_RadioSend(TELEMETRY_RADIO, (uint8_t*) &transmitPacket,
				RADIO_PACKET_SIZE);
	}

	if (currentTime - lastSent.positionLastSent >= 1000 / POSITION_RATE) {
		uint8_t pyroCont = 0;
		for (int i = 0; i < sizeof(sensorData->pyro_continuity); i++)
			pyroCont |= ((sensorData->pyro_continuity[i] & 0x01) << i);

		PositionPacket_t data = { sensorData->baro1_temp, filterData->pos_z,
				filterData->vel_z, sensorData->gps_lat, sensorData->gps_long,
				sensorData->gps_alt, sensorData->battery_voltage,
				sensorData->gps_speed, sensorData->gps_course,
				sensorData->gps_timestamp, sensorData->gps_num_sats, pyroCont, // filled in in a second
				state, 0 // TODO bluetooth clients
				};

		transmitPacket.packetType = TELEMETRY_ID_POSITION;
		transmitPacket.payload.positionData = data;
		lastSent.positionLastSent = currentTime;

		HM_RadioSend(TELEMETRY_RADIO, (uint8_t*) &transmitPacket,
				RADIO_PACKET_SIZE);
	}

	if (currentTime - lastSent.altInfoLastSent >= 1213) {
			AltInfoPacket_t data = {
				sensorData->baro1_pres,
				sensorData->baro2_pres,
				filterGetPressureRef(),
				cliGetConfigs()->groundElevationM,
				cliGetConfigs()->groundTemperatureC,
				cliGetConfigs()->mainCutAltitudeM
			};

			transmitPacket.packetType = TELEMETRY_ID_ALT_INFO;
			transmitPacket.payload.altitudeInfo = data;
			lastSent.altInfoLastSent = currentTime;

			HM_RadioSend(TELEMETRY_RADIO, (uint8_t*) &transmitPacket,
					RADIO_PACKET_SIZE);
		}

//	if (currentTime - lastSent.lineCutterLastSent >= 1000) {
//		LineCutterPacket_t data = {
//			sensorData->lineCutterNumber,
//			sensorData->state,
//			sensorData->battSense,
//			sensorData->cutSense1,
//			sensorData->cutSense2,
//			sensorData->currentSense,
//			sensorData->photoresistor,
//			sensorData->timestamp,
//			sensorData->pressure,
//			sensorData->altitude,
//			sensorData->avgAltitude,
//			sensorData->deltaAltitude,
//			sensorData->avgDeltaAltitude,
//			sensorData->temperature,
//			sensorData->accelX,
//			sensorData->accelY,
//			sensorData->accelZ
//		};
//		transmitPacket.packetType = 4;
//		transmitPacket.payload.lineCutter = data;
//		lastSent.lineCutterLastSent = currentTime;
//

}

#endif

#define min(a, b) (a < b) ? a : b

void RadioManager_transmitString(Hardware_t radio, uint8_t *data, size_t len) {
	while (len) {
		uint8_t txLen = min(len, RADIO_MAX_STRING);

		transmitPacket.timestampMs = HM_Millis();
		transmitPacket.packetType = TELEMETRY_ID_STRING;

		// copy txLen many bytes into the string, and set the rest to null chars
		memset(transmitPacket.payload.cliString.string, 0, RADIO_MAX_STRING);
		memcpy(transmitPacket.payload.cliString.string, data, txLen);
		transmitPacket.payload.cliString.len = txLen;

		HM_RadioSend(radio, (uint8_t*) &transmitPacket,
				RADIO_PACKET_SIZE);

		len -= txLen;
		data += txLen;
	}
}
