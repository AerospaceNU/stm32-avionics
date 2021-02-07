/*
 * data_log.c
 */
#include "data_log.h"

#include <stdbool.h>
#include <string.h>
#include "stm32h7xx_hal.h"

#define BOARD_VERSION 		0x01
#define BOARD_ID 			0x01
#define SOFTWARE_VERSION 	0x01

static uint32_t flightNum;
static uint32_t curSectorNum;
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
	double   baro1_temp,		baro1_pres,			baro1_alt;
	double   baro2_temp,		baro2_pres,			baro2_alt;
	double	 gps_lat,			gps_long,			gps_alt;
	double   battery_voltage;
	uint8_t  pyro_continuity;
	double	 heading,			vtg;
	double	 pos_x,				pos_y,				pos_z;
	double	 vel_x,				vel_y,				vel_z;
	double	 qx,				qy,					qz,					qw;
	uint8_t  state;
} LogData;

static LogData logPacket;

static const uint8_t kLogDataSize = sizeof(LogData);

void data_log_init() {

	uint32_t waitStartMS = 0;

	uint8_t rxBuffVersions[2] = {0};
	HM_FlashReadStart(0, 2, rxBuffVersions);
	uint8_t boardVersion = (rxBuffVersions[0] >> 4) & 0x0F;
	uint8_t boardID = rxBuffVersions[0] & 0x0F;
	uint8_t softwareVersion = rxBuffVersions[1];

	if (boardVersion != BOARD_VERSION || boardID != BOARD_ID || softwareVersion != SOFTWARE_VERSION) {
		// Erase sector since metadata indicates the versions are not the expected
		HM_FlashEraseSectorStart(0);
		waitStartMS = HM_Millis();
		while (!HM_FlashIsEraseComplete() && HM_Millis() - waitStartMS < FLASH_TIMEOUT_MS);
		// Write board version, id, and software version
		uint8_t txBuffVersions[] = {BOARD_VERSION << 4 | BOARD_ID, SOFTWARE_VERSION};
		HM_FlashWriteStart(0, 2, txBuffVersions);
		waitStartMS = HM_Millis();
		while (!HM_FlashIsWriteComplete() && HM_Millis() - waitStartMS < FLASH_TIMEOUT_MS);
		flightNum = 1;
		curSectorNum = 1;
	}
	else {
		// If board version/id/software version correct, figure out what flight it is
		bool flightNumFound = false;
		curSectorNum = 1;
		uint8_t sectorRxBuff[2] = {0};
		while (!flightNumFound) {
			HM_FlashReadStart(curSectorNum * 2, 2, sectorRxBuff);
			uint16_t tempFlightNum = (sectorRxBuff[0] << 8) | sectorRxBuff[1];
			if (tempFlightNum == 0xFFFF) {
				flightNumFound = true;
			}
			else {
				flightNum = tempFlightNum;
				curSectorNum++;
			}
		}
		flightNum++; // Add 1 to flight number for new flight number (most recent + 1)
	}

	// Erase sector where flight metadata will go
	HM_FlashEraseSectorStart(curSectorNum);
	waitStartMS = HM_Millis();
	while (!HM_FlashIsEraseComplete() && HM_Millis() - waitStartMS < FLASH_TIMEOUT_MS);

	// TODO: Write flight metadata

	// Write flight number and sector to metadata (since flight metadata written to it)
	uint8_t flightTxBuff[2] = {(flightNum >> 8) & 0xFF, flightNum & 0xFF};
	HM_FlashWriteStart(curSectorNum * 2, 2, flightTxBuff);
	waitStartMS = HM_Millis();
	while (!HM_FlashIsWriteComplete() && HM_Millis() - waitStartMS < FLASH_TIMEOUT_MS);
	HM_FlashReadStart(curSectorNum * 2, 2, flightTxBuff);

	curWriteAddress = curSectorNum * FLASH_SECTOR_BYTES + FLIGHT_METADATA_PAGES * FLASH_PAGE_SIZE_BYTES;
}

void data_log_write(SensorData_t* sensorData, uint8_t state) {

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
	logPacket.baro1_alt = sensorData->baro1_alt;
	logPacket.baro2_pres = sensorData->baro2_pres;
	logPacket.baro2_temp = sensorData->baro2_temp;
	logPacket.baro2_alt = sensorData->baro2_alt;
	logPacket.gps_lat = sensorData->gps_lat;
	logPacket.gps_long = sensorData->gps_long;
	logPacket.gps_alt = sensorData->gps_alt;
	logPacket.battery_voltage = sensorData->battery_voltage;
	logPacket.pyro_continuity = ((sensorData->pyro1_continuity & 0x01) << 5) |
						((sensorData->pyro2_continuity & 0x01) << 4) |
						((sensorData->pyro3_continuity & 0x01) << 3) |
						((sensorData->pyro4_continuity & 0x01) << 2) |
						((sensorData->pyro5_continuity & 0x01) << 1) |
						((sensorData->pyro6_continuity & 0x01));
	logPacket.heading = sensorData->heading;
	logPacket.vtg = sensorData->vtg;
	logPacket.pos_x = sensorData->pos_x;
	logPacket.pos_y = sensorData->pos_y;
	logPacket.pos_z = sensorData->pos_z;
	logPacket.vel_x = sensorData->vel_x;
	logPacket.vel_y = sensorData->vel_y;
	logPacket.vel_z = sensorData->vel_z;
	logPacket.qx = sensorData->qx;
	logPacket.qy = sensorData->qy;
	logPacket.qz = sensorData->qz;
	logPacket.qw = sensorData->qw;
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

uint32_t data_log_read(uint32_t flightNum, uint32_t maxBytes, uint8_t *pdata) {

	static uint32_t firstSector;
	static uint32_t lastSector;
	static uint32_t lastFlightNum;
	static uint32_t readOffset;
	// Find first sector of flight num using metadata if no offset specified
	if (flightNum != lastFlightNum) {
		firstSector = 0;
		lastSector = 0;
		lastFlightNum = flightNum;
		readOffset = 0;
		uint32_t tempCurSectorNum = 1;
		uint8_t sectorRxBuff[2] = {0};
		while (firstSector == 0 && tempCurSectorNum < FLASH_SIZE_BYTES / FLASH_SECTOR_BYTES) {
			HM_FlashReadStart(tempCurSectorNum * 2, 2, sectorRxBuff);
			uint16_t tempFlightNum = (sectorRxBuff[0] << 8) | sectorRxBuff[1];
			if (tempFlightNum == flightNum) {
				firstSector = tempCurSectorNum;
			}
			tempCurSectorNum++;
		}

		// If the first sector is 0 after the while loop, the flight number doesn't exist, so nothing is read
		if (firstSector == 0) {
			return 0;
		}

		// Find last sector of flight num using metadata. This helps know when to stop reading in some situations
		while (lastSector == 0 && tempCurSectorNum < FLASH_SIZE_BYTES / FLASH_SECTOR_BYTES) {
			HM_FlashReadStart(tempCurSectorNum * 2, 2, sectorRxBuff);
			uint16_t tempFlightNum = (sectorRxBuff[0] << 8) | sectorRxBuff[1];
			if (tempFlightNum != flightNum) {
				lastSector = tempCurSectorNum - 1;
			}
			tempCurSectorNum++;
		}

		// If the last sector is still 0, the last sector of data is the last sector of flash
		if (lastSector == 0) {
			lastSector = FLASH_SIZE_BYTES / FLASH_SECTOR_BYTES;
		}
	}

	// Read from flight at the appropriate offset (excluding metadata)
	uint32_t readAddress = firstSector * FLASH_SECTOR_BYTES + FLIGHT_METADATA_PAGES * FLASH_PAGE_SIZE_BYTES + readOffset;
	HM_FlashReadStart(readAddress, maxBytes, pdata);
	// If the last sector has been passed, return only the bytes that count
	if ((readAddress + maxBytes) > FLASH_SECTOR_BYTES * (lastSector + 1)) {
		return (lastSector + 1) * FLASH_SECTOR_BYTES - readAddress;
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
