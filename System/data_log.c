/*
 * data_log.c
 */
#include "data_log.h"

#include <stdbool.h>
#include <string.h>

#ifdef FCB_VERSION
const uint8_t BOARD_VERSION = FCB_VERSION;
#else
const uint8_t BOARD_VERSION = 0xff;
#endif
#define BOARD_ID 			0x01
#define SOFTWARE_VERSION 	0x01
#define FLASH_TIMEOUT       500

static uint32_t flightNum = 0;
static uint32_t curSectorNum;
static uint32_t flightFirstSectorNum;
static uint32_t curWriteAddress;

// Log Data shows what data will be logged in what relative location
typedef struct __attribute__((__packed__)) LogData {
	uint32_t timestamp_s, 		timestamp_us;
	int16_t  imu1_accel_x_raw, 	imu1_accel_y_raw, 	imu1_accel_z_raw;
	int16_t  imu1_gyro_x_raw, 	imu1_gyro_y_raw, 	imu1_gyro_z_raw;
	int16_t  imu1_mag_x_raw, 	imu1_mag_y_raw, 	imu1_mag_z_raw;
	int16_t  imu2_accel_x_raw, 	imu2_accel_y_raw, 	imu2_accel_z_raw;
	int16_t  imu2_gyro_x_raw, 	imu2_gyro_y_raw, 	imu2_gyro_z_raw;
	int16_t  imu2_mag_x_raw,	imu2_mag_y_raw,		imu2_mag_z_raw;
	int16_t  high_g_accel_x_raw,high_g_accel_y_raw,	high_g_accel_z_raw;
	double   baro1_temp,		baro1_pres;
	double   baro2_temp,		baro2_pres;
	float	 gps_lat,			gps_long,			gps_alt;
	double   battery_voltage;
	uint8_t  pyro_continuity;
	double	 heading,			vtg;
	double	 pos_x,				pos_y,				pos_z;
	double	 vel_x,				vel_y,				vel_z;
	double	 qx,				qy,					qz,					qw;
	uint8_t  state;
} LogData;

static LogData logPacket;
static FlightMetadata metadataPacket;

static const uint8_t kLogDataSize = sizeof(LogData);

static const uint8_t kFlightMetadataSize = sizeof(FlightMetadata);

static uint32_t data_log_get_last_flight_num_type(bool launched) {

	uint32_t lastFlightNum = 0;

	uint8_t rxBuffVersions[2] = {0};
	HM_FlashReadStart(0, 2, rxBuffVersions);
	uint32_t waitStartMS = HM_Millis();
	while (!HM_FlashIsReadComplete() && HM_Millis() - waitStartMS < FLASH_TIMEOUT);
	uint8_t boardVersion = (rxBuffVersions[0] >> 4) & 0x0F;
	uint8_t boardID = rxBuffVersions[0] & 0x0F;
	uint8_t softwareVersion = rxBuffVersions[1];

	if (boardVersion != BOARD_VERSION || boardID != BOARD_ID || softwareVersion != SOFTWARE_VERSION) {
		lastFlightNum = 0;
		curSectorNum = 1;
	}
	else {
		// If board version/id/software version correct, figure out what flight it is
		bool flightNumFound = false;
		curSectorNum = 1;
		uint8_t sectorRxBuff[2] = {0};
		uint8_t metadataBuff[kFlightMetadataSize]; // Create a buffer for the metadata
		uint32_t metadataReadAddress;
		while (!flightNumFound) {
			HM_FlashReadStart(curSectorNum * 2, 2, sectorRxBuff);
			waitStartMS = HM_Millis();
			while (!HM_FlashIsReadComplete() && HM_Millis() - waitStartMS < FLASH_TIMEOUT);
			uint16_t tempFlightNum = (sectorRxBuff[0] << 8) | sectorRxBuff[1];
			if (tempFlightNum == 0xFFFF) {
				flightNumFound = true;
			}
			else {
				if (launched) { // Need to check for launched to mark as last flight
					metadataReadAddress = curSectorNum * FLASH_SECTOR_BYTES;
					HM_FlashReadStart(metadataReadAddress, kFlightMetadataSize, metadataBuff); // Read the metadata
					waitStartMS = HM_Millis();
					while (!HM_FlashIsReadComplete() && HM_Millis() - waitStartMS < FLASH_TIMEOUT);
					metadataPacket = *(FlightMetadata*)&metadataBuff;
					if (metadataPacket.launched == 1) { //Only update last if it was launched
						lastFlightNum = tempFlightNum;
					}
				} else {
					lastFlightNum = tempFlightNum;
				}
				curSectorNum++;
			}
		}
	}

	return lastFlightNum;
}

uint32_t data_log_get_last_flight_num() {
	return data_log_get_last_flight_num_type(false);
}


uint32_t data_log_get_last_launched_flight_num() {
	return data_log_get_last_flight_num_type(true);
}

static void data_log_get_flight_sectors(uint32_t flightNum, uint32_t *firstSector, uint32_t *lastSector) {
	*firstSector = 0;
	*lastSector = 0;
	uint32_t tempCurSectorNum = 1;
	uint8_t sectorRxBuff[2] = {0};
	uint32_t waitStartMS;
	while (*firstSector == 0 && tempCurSectorNum < FLASH_SIZE_BYTES / FLASH_SECTOR_BYTES) {
		HM_FlashReadStart(tempCurSectorNum * 2, 2, sectorRxBuff);
		waitStartMS = HM_Millis();
		while (!HM_FlashIsReadComplete() && HM_Millis() - waitStartMS < FLASH_TIMEOUT);
		uint16_t tempFlightNum = (sectorRxBuff[0] << 8) | sectorRxBuff[1];
		if (tempFlightNum == flightNum) {
			*firstSector = tempCurSectorNum;
		}
		tempCurSectorNum++;
	}

	// If the first sector is 0 after the while loop, the flight number doesn't exist
	if (*firstSector == 0) {
		return;
	}

	// Find last sector of flight num using metadata.
	while (*lastSector == 0 && tempCurSectorNum < FLASH_SIZE_BYTES / FLASH_SECTOR_BYTES) {
		HM_FlashReadStart(tempCurSectorNum * 2, 2, sectorRxBuff);
		waitStartMS = HM_Millis();
		while (!HM_FlashIsReadComplete() && HM_Millis() - waitStartMS < FLASH_TIMEOUT);
		uint16_t tempFlightNum = (sectorRxBuff[0] << 8) | sectorRxBuff[1];
		if (tempFlightNum != flightNum) {
			*lastSector = tempCurSectorNum - 1;
		}
		tempCurSectorNum++;
	}

	// If the last sector is still 0, the last sector of data is the last sector of flash
	if (*lastSector == 0) {
		*lastSector = FLASH_SIZE_BYTES / FLASH_SECTOR_BYTES;
	}
}

void data_log_load_last_stored_flight_metadata() {
	flightNum = data_log_get_last_flight_num(); // Load the previous flight number and sector
	uint8_t metadataBuff[kFlightMetadataSize]; // Create a buffer for the metadata
	uint32_t prevFirstSector, prevLastSector;
	data_log_get_flight_sectors(flightNum - 1, &prevFirstSector, &prevLastSector);
	uint32_t metadataReadAddress = prevFirstSector * FLASH_SECTOR_BYTES;
	HM_FlashReadStart(metadataReadAddress, kFlightMetadataSize, metadataBuff); // Read the metadata
	uint32_t waitStartMS = HM_Millis();
	while (!HM_FlashIsReadComplete() && HM_Millis() - waitStartMS < FLASH_TIMEOUT);
	metadataPacket = *(FlightMetadata*)&metadataBuff;
}

void data_log_get_flight_metadata(FlightMetadata *metadata, uint8_t flightNum) {
	bool flightNumFound = false;
	uint32_t sectorNum = 1;
	uint8_t sectorRxBuff[2] = {0};
	uint32_t waitStartMS;
	while (!flightNumFound) {
		HM_FlashReadStart(sectorNum * 2, 2, sectorRxBuff);
		waitStartMS = HM_Millis();
		while (!HM_FlashIsReadComplete() && HM_Millis() - waitStartMS < FLASH_TIMEOUT);
		uint16_t tempFlightNum = (sectorRxBuff[0] << 8) | sectorRxBuff[1];
		if (tempFlightNum == flightNum) {
			flightNumFound = true;
		}
		else {
			sectorNum++;
		}
	}
	uint8_t metadataBuff[kFlightMetadataSize]; // Create a buffer for the metadata
	uint32_t metadataReadAddress = sectorNum * FLASH_SECTOR_BYTES;
	HM_FlashReadStart(metadataReadAddress, kFlightMetadataSize, metadataBuff); // Read the metadata
	waitStartMS = HM_Millis();
	while (!HM_FlashIsReadComplete() && HM_Millis() - waitStartMS < FLASH_TIMEOUT);
	metadataPacket = *(FlightMetadata*)&metadataBuff;
	*metadata = *(FlightMetadata*)&metadataBuff;
}

FlightMetadata data_log_get_metadata() {
	return metadataPacket;
}

void data_log_assign_flight() {

	uint32_t waitStartMS = 0;

	// Assign flight number
	flightNum = data_log_get_last_flight_num() + 1;
	if (flightNum == 1) {
		// Erase sector since metadata indicates the versions are not the expected
		HM_FlashEraseSectorStart(0);
		waitStartMS = HM_Millis();
		while (!HM_FlashIsEraseComplete() && HM_Millis() - waitStartMS < FLASH_TIMEOUT_MS);
		// Write board version, id, and software version
		uint8_t txBuffVersions[] = {BOARD_VERSION << 4 | BOARD_ID, SOFTWARE_VERSION};
		HM_FlashWriteStart(0, 2, txBuffVersions);
		waitStartMS = HM_Millis();
		while (!HM_FlashIsWriteComplete() && HM_Millis() - waitStartMS < FLASH_TIMEOUT_MS);
	}

	// Erase sector where flight metadata will go
	HM_FlashEraseSectorStart(curSectorNum);
	waitStartMS = HM_Millis();
	while (!HM_FlashIsEraseComplete() && HM_Millis() - waitStartMS < FLASH_TIMEOUT_MS);

	// TODO: Write flight metadata

	// Empty metadata packet
	memset(&metadataPacket, 0xFF, kFlightMetadataSize);
	data_log_write_metadata();

	// Write flight number and sector to metadata (since flight metadata written to it)
	uint8_t flightTxBuff[2] = {(flightNum >> 8) & 0xFF, flightNum & 0xFF};
	HM_FlashWriteStart(curSectorNum * 2, 2, flightTxBuff);
	waitStartMS = HM_Millis();
	while (!HM_FlashIsWriteComplete() && HM_Millis() - waitStartMS < FLASH_TIMEOUT_MS);
	HM_FlashReadStart(curSectorNum * 2, 2, flightTxBuff);
	waitStartMS = HM_Millis();
	while (!HM_FlashIsReadComplete() && HM_Millis() - waitStartMS < FLASH_TIMEOUT);

	flightFirstSectorNum = curSectorNum;
	curWriteAddress = curSectorNum * FLASH_SECTOR_BYTES + FLIGHT_METADATA_PAGES * FLASH_PAGE_SIZE_BYTES;
}

void data_log_set_pressure_metadata(double presRef) {
	if (flightNum > 0) {
		metadataPacket.pressureRef = presRef; // Set the correct pressure reference
	}
}

void data_log_set_launched_metadata() {
	if (flightNum > 0) {
		metadataPacket.launched = 1; // Indicate that the current flight was launched (transitioned past preflight)
	}
}

void data_log_set_timestamp_metadata(uint64_t timestamp) {
	if (flightNum > 0) {
		metadataPacket.timestamp = timestamp;
	}
}

void data_log_write_metadata() {
	if (flightNum > 0) {
		uint32_t metadataWriteAddress = flightFirstSectorNum * FLASH_SECTOR_BYTES; // Metadata is located at the start of the flight sector
		HM_FlashWriteStart(metadataWriteAddress, kFlightMetadataSize, (uint8_t*)&metadataPacket); // Write the metadata packet
		uint32_t waitStartMS = HM_Millis();
		while (!HM_FlashIsWriteComplete() && HM_Millis() - waitStartMS < FLASH_TIMEOUT_MS);
	}
}

void data_log_copy_metadata(FlightMetadata *oldMetadataPacket) {
	if (flightNum > 0) {
		memcpy(&metadataPacket, oldMetadataPacket, kFlightMetadataSize);
	}
}

void data_log_write(SensorData_t* sensorData, FilterData_t* filterData, uint8_t state) {

	if (flightNum > 0) {

		uint32_t waitStartMS = 0;

		logPacket.timestamp_s = sensorData->timestamp_s;
		logPacket.timestamp_us = sensorData->timestamp_us;
		logPacket.imu1_accel_x_raw = sensorData->imu1_accel_x_raw;
		logPacket.imu1_accel_y_raw = sensorData->imu1_accel_y_raw;
		logPacket.imu1_accel_z_raw = sensorData->imu1_accel_z_raw;
		logPacket.imu1_gyro_x_raw = sensorData->imu1_gyro_x_raw;
		logPacket.imu1_gyro_y_raw = sensorData->imu1_gyro_y_raw;
		logPacket.imu1_gyro_z_raw = sensorData->imu1_gyro_z_raw;
		logPacket.imu1_mag_x_raw = sensorData->imu1_mag_x_raw;
		logPacket.imu1_mag_y_raw = sensorData->imu1_mag_y_raw;
		logPacket.imu1_mag_z_raw = sensorData->imu1_mag_z_raw;
		logPacket.imu2_accel_x_raw = sensorData->imu2_accel_x_raw;
		logPacket.imu2_accel_y_raw = sensorData->imu2_accel_y_raw;
		logPacket.imu2_accel_z_raw = sensorData->imu2_accel_z_raw;
		logPacket.imu2_gyro_x_raw = sensorData->imu2_gyro_x_raw;
		logPacket.imu2_gyro_y_raw = sensorData->imu2_gyro_y_raw;
		logPacket.imu2_gyro_z_raw = sensorData->imu2_gyro_z_raw;
		logPacket.imu2_mag_x_raw = sensorData->imu2_mag_x_raw;
		logPacket.imu2_mag_y_raw = sensorData->imu2_mag_y_raw;
		logPacket.imu2_mag_z_raw = sensorData->imu2_mag_z_raw;
		logPacket.high_g_accel_x_raw = sensorData->high_g_accel_x_raw;
		logPacket.high_g_accel_y_raw = sensorData->high_g_accel_y_raw;
		logPacket.high_g_accel_z_raw = sensorData->high_g_accel_z_raw;
		logPacket.baro1_pres = sensorData->baro1_pres;
		logPacket.baro1_temp = sensorData->baro1_temp;
		logPacket.baro2_pres = sensorData->baro2_pres;
		logPacket.baro2_temp = sensorData->baro2_temp;
		logPacket.gps_lat = sensorData->gps_lat;
		logPacket.gps_long = sensorData->gps_long;
		logPacket.gps_alt = sensorData->gps_alt;
		logPacket.battery_voltage = sensorData->battery_voltage;
		logPacket.pyro_continuity = 0;
		for (int i = 0; i < sizeof(sensorData->pyro_continuity); i++) {
			logPacket.pyro_continuity |= ((sensorData->pyro_continuity[i] & 0x01) << i);
		}
		logPacket.heading = filterData->heading;
		logPacket.vtg = filterData->vtg;
		logPacket.pos_x = filterData->pos_x;
		logPacket.pos_y = filterData->pos_y;
		logPacket.pos_z = filterData->pos_z;
		logPacket.vel_x = filterData->vel_x;
		logPacket.vel_y = filterData->vel_y;
		logPacket.vel_z = filterData->vel_z;
		logPacket.qx = filterData->qx;
		logPacket.qy = filterData->qy;
		logPacket.qz = filterData->qz;
		logPacket.qw = filterData->qw;
		logPacket.state = state;

		// Find what pages to write packet on
		uint32_t second_page_bytes = (curWriteAddress + kLogDataSize) % FLASH_PAGE_SIZE_BYTES;
		if (second_page_bytes >= kLogDataSize)
			second_page_bytes = 0;
		uint32_t first_page_bytes = kLogDataSize - second_page_bytes;

		// Check if a new sector has been entered. If so, record in file system and erase the sector
		if ((curWriteAddress + kLogDataSize) / FLASH_SECTOR_BYTES > curSectorNum) {
			curSectorNum++;
			// Write flight number and sector to metadata
			uint8_t flightTxBuff[2] = {(flightNum >> 8) & 0xFF, flightNum & 0xFF};
			HM_FlashWriteStart(curSectorNum * 2, 2, flightTxBuff);
			waitStartMS = HM_Millis();
			while (!HM_FlashIsWriteComplete() && HM_Millis() - waitStartMS < FLASH_TIMEOUT_MS);
			// Erase the upcoming sector
			HM_FlashEraseSectorStart(curSectorNum);
			waitStartMS = HM_Millis();
			while (!HM_FlashIsEraseComplete() && HM_Millis() - waitStartMS < FLASH_TIMEOUT_MS);
		}

		// Write to first page and, if necessary, second page
		HM_FlashWriteStart(curWriteAddress, first_page_bytes, (uint8_t*) &logPacket);
		waitStartMS = HM_Millis();
		while (!HM_FlashIsWriteComplete() && HM_Millis() - waitStartMS < FLASH_TIMEOUT_MS);
		if (second_page_bytes > 0) {
			HM_FlashWriteStart(curWriteAddress + first_page_bytes, second_page_bytes, (uint8_t*) &logPacket + first_page_bytes);
			waitStartMS = HM_Millis();
			while (!HM_FlashIsWriteComplete() && HM_Millis() - waitStartMS < FLASH_TIMEOUT_MS);
		}
		// Increment write address to be next address to write to
		curWriteAddress += kLogDataSize;
	}
}

uint32_t data_log_read(uint32_t flightNum, uint32_t maxBytes, uint8_t *pdata, bool reset) {

	static uint32_t firstSector;
	static uint32_t lastSector;
	static uint32_t lastFlightNum;
	static uint32_t readOffset;
	// Find sectors of flight num using metadata if no offset specified
	if ((flightNum != lastFlightNum) || reset) {
		lastFlightNum = flightNum;
		readOffset = 0;
		data_log_get_flight_sectors(flightNum, &firstSector, &lastSector);
	}

	// Read from flight at the appropriate offset (excluding metadata)
	uint32_t readAddress = firstSector * FLASH_SECTOR_BYTES + FLIGHT_METADATA_PAGES * FLASH_PAGE_SIZE_BYTES + readOffset;
	HM_FlashReadStart(readAddress, maxBytes, pdata);
	uint32_t waitStartMS = HM_Millis();
	while (!HM_FlashIsReadComplete() && HM_Millis() - waitStartMS < FLASH_TIMEOUT);
	// If the last sector has been passed, return only the bytes that count
	if ((readAddress + maxBytes) > FLASH_SECTOR_BYTES * (lastSector + 1)) {
		uint32_t bytesRead = (lastSector + 1) * FLASH_SECTOR_BYTES - readAddress;
		readOffset += bytesRead;
		return bytesRead;
	}
	// If all bytes are 0xFF, return that none count since that part of the log wasn't written to
	// Only apply this if the max read bytes is large enough. Otherwise, the 0xFF could be caused by the data
	if (maxBytes > 20) {
		for (int i = 0; i < maxBytes; i++) {
			if (*(pdata + i) != 0xFF)
				break;
			if (i == maxBytes - 1) {
				return 0;
			}
		}
	}

	readOffset += maxBytes;
	return maxBytes;
}

void data_log_get_last_data_packet(uint32_t flightNum, LogData *flightPacket) {
	uint8_t logPacketBuffer[kLogDataSize];
	uint32_t firstSector;
	uint32_t lastSector;

	// Find sectors of flight num using metadata
	data_log_get_flight_sectors(flightNum, &firstSector, &lastSector);

	// Binary search to find the last data packet
	uint32_t low = 0;
	uint32_t high = (FLASH_SECTOR_BYTES * (lastSector - firstSector + 1)) / kLogDataSize;
	uint32_t mid = (high + low) / 2;

	uint32_t readAddress = firstSector * FLASH_SECTOR_BYTES + FLIGHT_METADATA_PAGES * FLASH_PAGE_SIZE_BYTES + mid * kLogDataSize;
	HM_FlashReadStart(readAddress, kLogDataSize, logPacketBuffer);
	uint32_t waitStartMS = HM_Millis();
	while (!HM_FlashIsReadComplete() && HM_Millis() - waitStartMS < FLASH_TIMEOUT);
	LogData *tempPacket = (LogData *)logPacketBuffer;

	while (low < mid) {
		// Check that the state is reasonable, as the last part of the packet
		if (!(tempPacket->state >= 0 && tempPacket->state <= 50)) {
			high = mid;
			mid = (high + low) / 2;
		}
		else {
			low = mid;
			mid = (high + low) / 2;
		}
		readAddress = firstSector * FLASH_SECTOR_BYTES + FLIGHT_METADATA_PAGES * FLASH_PAGE_SIZE_BYTES + mid * kLogDataSize;
		HM_FlashReadStart(readAddress, kLogDataSize, logPacketBuffer);
		waitStartMS = HM_Millis();
		while (!HM_FlashIsReadComplete() && HM_Millis() - waitStartMS < FLASH_TIMEOUT);
		tempPacket = (LogData *)logPacketBuffer;
	}

	*flightPacket = *tempPacket;
}

uint32_t data_log_get_last_flight_timestamp(uint32_t flightNum) {

	LogData dataPacket;
	data_log_get_last_data_packet(flightNum, &dataPacket);

	return dataPacket.timestamp_s;
}

uint8_t data_log_get_flash_usage() {
	if (flightNum == 0) { // In a cli state, curWriteAddress is not valid
		data_log_get_last_flight_num();
		curWriteAddress = curSectorNum * FLASH_SECTOR_BYTES + FLIGHT_METADATA_PAGES * FLASH_PAGE_SIZE_BYTES;
	}
	uint8_t usage = (uint8_t)(curWriteAddress / (FLASH_SIZE_BYTES / 100.0));
	return usage;
}
