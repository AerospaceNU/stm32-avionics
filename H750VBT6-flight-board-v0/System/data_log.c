/*
 * data_log.c
 *
Data Format:
timestamp_s		int32_t
timestamp_us	int32_t
imu1_accel_x	int16_t
imu1_accel_y	int16_t
imu1_accel_z	int16_t
imu1_gyro_x		int16_t
imu1_gyro_y		int16_t
imu1_gyro_z		int16_t
imu1_mag_x		int16_t
imu1_mag_y		int16_t
imu1_mag_z		int16_t
imu2_accel_x	int16_t
imu2_accel_y	int16_t
imu2_accel_z	int16_t
imu2_gyro_x		int16_t
imu2_gyro_y		int16_t
imu2_gyro_z		int16_t
imu2_mag_x		int16_t
imu2_mag_y		int16_t
imu2_mag_z		int16_t
high_g_accel_x	int16_t
high_g_accel_y	int16_t
high_g_accel_z	int16_t
baro1_temp		double
baro1_press		double
baro1_height	double
baro2_temp		double
baro2_press		double
baro2_height	double
 */

#include <stdbool.h>
#include "data_log.h"
#include "S25FLx.h"

#define SECTOR_BYTES 			0x40000
#define FLIGHT_METADATA_PAGES 	2
#define FLASH_SIZE_BYTES 		0x40000000
#define FLASH_PAGE_SIZE_BYTES	0x200

#define BOARD_VERSION 		0x01
#define BOARD_ID 			0x01
#define SOFTWARE_VERSION 	0x01

#define FLASH_TIMEOUT_MS 	500
#define LOG_PACKET_BYTES	122

static S25FLXCtrl_t flash;
static uint32_t flightNum;
static uint32_t curSectorNum;
static uint32_t curWriteAddress;

void data_log_init(SPI_HandleTypeDef *hspi, GPIO_TypeDef *csPort, uint16_t csPin) {

	uint32_t waitStartMS = 0;

	S25FLX_init(&flash, hspi, csPort, csPin, FLASH_SIZE_BYTES);

	uint8_t rxBuffVersions[2] = {0};
	S25FLX_read_start(&flash, 0, 2, rxBuffVersions);
	uint8_t boardVersion = (rxBuffVersions[0] >> 4) & 0x0F;
	uint8_t boardID = rxBuffVersions[0] & 0x0F;
	uint8_t softwareVersion = rxBuffVersions[1];

	if (boardVersion != BOARD_VERSION || boardID != BOARD_ID || softwareVersion != SOFTWARE_VERSION) {
		// Erase sector since metadata indicates the versions are not the expected
		S25FLX_erase_sector_start(&flash, 0);
		waitStartMS = HAL_GetTick();
		while (!S25FLX_is_erase_complete(&flash) && HAL_GetTick() - waitStartMS < FLASH_TIMEOUT_MS);
		// Write board version, id, and software version
		uint8_t txBuffVersions[] = {BOARD_VERSION << 4 | BOARD_ID, SOFTWARE_VERSION};
		S25FLX_write_start(&flash, 0, 2, txBuffVersions);
		waitStartMS = HAL_GetTick();
		while (!S25FLX_is_write_complete(&flash) && HAL_GetTick() - waitStartMS < FLASH_TIMEOUT_MS);
		flightNum = 1;
		curSectorNum = 1;
	}
	else {
		// If board version/id/software version correct, figure out what flight it is
		bool flightNumFound = false;
		curSectorNum = 1;
		uint8_t sectorRxBuff[2] = {0};
		while (!flightNumFound) {
			S25FLX_read_start(&flash, curSectorNum * 2, 2, sectorRxBuff);
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
	S25FLX_erase_sector_start(&flash, curSectorNum);
	waitStartMS = HAL_GetTick();
	while (!S25FLX_is_erase_complete(&flash) && HAL_GetTick() - waitStartMS < FLASH_TIMEOUT_MS);

	// TODO: Write flight metadata

	// Write flight number and sector to metadata (since flight metadata written to it)
	uint8_t flightTxBuff[2] = {(flightNum >> 8) & 0xFF, flightNum & 0xFF};
	S25FLX_write_start(&flash, curSectorNum * 2, 2, flightTxBuff);
	waitStartMS = HAL_GetTick();
	while (!S25FLX_is_write_complete(&flash) && HAL_GetTick() - waitStartMS < FLASH_TIMEOUT_MS);
	S25FLX_read_start(&flash, curSectorNum * 2, 2, flightTxBuff);

	curWriteAddress = curSectorNum * SECTOR_BYTES + FLIGHT_METADATA_PAGES * FLASH_PAGE_SIZE_BYTES;
}

void data_log_write(LSM9DS1Ctrl_t *imu1_data, LSM9DS1Ctrl_t *imu2_data, H3LIS331DLCtrl_t *high_g_data, MS5607Ctrl_t *baro1_data, MS5607Ctrl_t *baro2_data) {

	uint8_t logPacket[LOG_PACKET_BYTES] = {0};
	uint8_t* tempDouble;
	uint32_t waitStartMS = 0;

	// TODO: Add timestamp data to packet (bytes 0-7)

	// Add IMU1 data to packet (bytes 8-25)
	logPacket[8] = (imu1_data->ag.aRawVal.x >> 8) & 0xFF;
	logPacket[9] = (imu1_data->ag.aRawVal.x) & 0xFF;
	logPacket[10] = (imu1_data->ag.aRawVal.y >> 8) & 0xFF;
	logPacket[11] = (imu1_data->ag.aRawVal.y) & 0xFF;
	logPacket[12] = (imu1_data->ag.aRawVal.z >> 8) & 0xFF;
	logPacket[13] = (imu1_data->ag.aRawVal.z) & 0xFF;
	logPacket[14] = (imu1_data->ag.gRawVal.x >> 8) & 0xFF;
	logPacket[15] = (imu1_data->ag.gRawVal.x) & 0xFF;
	logPacket[16] = (imu1_data->ag.gRawVal.y >> 8) & 0xFF;
	logPacket[17] = (imu1_data->ag.gRawVal.y) & 0xFF;
	logPacket[18] = (imu1_data->ag.gRawVal.z >> 8) & 0xFF;
	logPacket[19] = (imu1_data->ag.gRawVal.z) & 0xFF;
	logPacket[20] = (imu1_data->m.mRawVal.x >> 8) & 0xFF;
	logPacket[21] = (imu1_data->m.mRawVal.x) & 0xFF;
	logPacket[22] = (imu1_data->m.mRawVal.y >> 8) & 0xFF;
	logPacket[23] = (imu1_data->m.mRawVal.y) & 0xFF;
	logPacket[24] = (imu1_data->m.mRawVal.z >> 8) & 0xFF;
	logPacket[25] = (imu1_data->m.mRawVal.z) & 0xFF;

	// Add IMU2 data to packet (bytes 26-43)
	logPacket[26] = (imu2_data->ag.aRawVal.x >> 8) & 0xFF;
	logPacket[27] = (imu2_data->ag.aRawVal.x) & 0xFF;
	logPacket[28] = (imu2_data->ag.aRawVal.y >> 8) & 0xFF;
	logPacket[29] = (imu2_data->ag.aRawVal.y) & 0xFF;
	logPacket[30] = (imu2_data->ag.aRawVal.z >> 8) & 0xFF;
	logPacket[31] = (imu2_data->ag.aRawVal.z) & 0xFF;
	logPacket[32] = (imu2_data->ag.gRawVal.x >> 8) & 0xFF;
	logPacket[33] = (imu2_data->ag.gRawVal.x) & 0xFF;
	logPacket[34] = (imu2_data->ag.gRawVal.y >> 8) & 0xFF;
	logPacket[35] = (imu2_data->ag.gRawVal.y) & 0xFF;
	logPacket[36] = (imu2_data->ag.gRawVal.z >> 8) & 0xFF;
	logPacket[37] = (imu2_data->ag.gRawVal.z) & 0xFF;
	logPacket[38] = (imu2_data->m.mRawVal.x >> 8) & 0xFF;
	logPacket[39] = (imu2_data->m.mRawVal.x) & 0xFF;
	logPacket[40] = (imu2_data->m.mRawVal.y >> 8) & 0xFF;
	logPacket[41] = (imu2_data->m.mRawVal.y) & 0xFF;
	logPacket[42] = (imu2_data->m.mRawVal.z >> 8) & 0xFF;
	logPacket[43] = (imu2_data->m.mRawVal.z) & 0xFF;

	// Add High G Accel data to packet (bytes 44-49)
	logPacket[44] = (high_g_data->rawVal.x >> 8) & 0xFF;
	logPacket[45] = high_g_data->rawVal.x & 0xFF;
	logPacket[46] = (high_g_data->rawVal.y >> 8) & 0xFF;
	logPacket[47] = high_g_data->rawVal.y & 0xFF;
	logPacket[48] = (high_g_data->rawVal.z >> 8) & 0xFF;
	logPacket[49] = high_g_data->rawVal.z & 0xFF;

	// Add barometer 1 data to packet (bytes 50-73)
	tempDouble = ((uint8_t *)&baro1_data->altData.temp);
	for (int i = 0; i < 8; i++)
		logPacket[50 + i] = *(tempDouble + i);
	tempDouble = ((uint8_t *)&baro1_data->altData.baro);
	for (int i = 0; i < 8; i++)
		logPacket[58 + i] = *(tempDouble + i);
	tempDouble = ((uint8_t *)&baro1_data->altData.alt);
	for (int i = 0; i < 8; i++)
		logPacket[66 + i] = *(tempDouble + i);

	// Add barometer 2 data to packet (bytes 74-97)
	tempDouble = ((uint8_t *)&baro2_data->altData.temp);
	for (int i = 0; i < 8; i++)
		logPacket[74 + i] = *(tempDouble + i);
	tempDouble = ((uint8_t *)&baro2_data->altData.baro);
	for (int i = 0; i < 8; i++)
		logPacket[82 + i] = *(tempDouble + i);
	tempDouble = ((uint8_t *)&baro2_data->altData.alt);
	for (int i = 0; i < 8; i++)
		logPacket[90 + i] = *(tempDouble + i);

	// TODO: Add GPS data to packet (bytes 98-121)

	// Find what pages to write packet on
	uint32_t second_page_bytes = (curWriteAddress + LOG_PACKET_BYTES) % FLASH_PAGE_SIZE_BYTES;
	if (second_page_bytes >= LOG_PACKET_BYTES)
		second_page_bytes = 0;
	uint32_t first_page_bytes = LOG_PACKET_BYTES - second_page_bytes;

	// Check if a new sector has been entered. If so, record in file system and erase the sector
	if ((curWriteAddress + LOG_PACKET_BYTES) / SECTOR_BYTES > curSectorNum) {
		curSectorNum++;
		// Write flight number and sector to metadata
		uint8_t flightTxBuff[2] = {(flightNum >> 8) & 0xFF, flightNum & 0xFF};
		S25FLX_write_start(&flash, curSectorNum * 2, 2, flightTxBuff);
		waitStartMS = HAL_GetTick();
		while (!S25FLX_is_write_complete(&flash) && HAL_GetTick() - waitStartMS < FLASH_TIMEOUT_MS);
		// Erase the upcoming sector
		S25FLX_erase_sector_start(&flash, curSectorNum);
		waitStartMS = HAL_GetTick();
		while (!S25FLX_is_write_complete(&flash) && HAL_GetTick() - waitStartMS < FLASH_TIMEOUT_MS);
	}

	// Write to first page and, if necessary, second page
	S25FLX_write_start(&flash, curWriteAddress, first_page_bytes, logPacket);
	waitStartMS = HAL_GetTick();
	while (!S25FLX_is_write_complete(&flash) && HAL_GetTick() - waitStartMS < FLASH_TIMEOUT_MS);
	if (second_page_bytes > 0) {
		S25FLX_write_start(&flash, curWriteAddress + first_page_bytes, second_page_bytes, logPacket + first_page_bytes);
		waitStartMS = HAL_GetTick();
		while (!S25FLX_is_write_complete(&flash) && HAL_GetTick() - waitStartMS < FLASH_TIMEOUT_MS);
	}
	// Increment write address to be next address to write to
	curWriteAddress += LOG_PACKET_BYTES;
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
		while (firstSector == 0 && tempCurSectorNum < FLASH_SIZE_BYTES / SECTOR_BYTES) {
			S25FLX_read_start(&flash, tempCurSectorNum * 2, 2, sectorRxBuff);
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
		while (lastSector == 0 && tempCurSectorNum < FLASH_SIZE_BYTES / SECTOR_BYTES) {
			S25FLX_read_start(&flash, tempCurSectorNum * 2, 2, sectorRxBuff);
			uint16_t tempFlightNum = (sectorRxBuff[0] << 8) | sectorRxBuff[1];
			if (tempFlightNum != flightNum) {
				lastSector = tempCurSectorNum - 1;
			}
			tempCurSectorNum++;
		}

		// If the last sector is still 0, the last sector of data is the last sector of flash
		if (lastSector == 0) {
			lastSector = FLASH_SIZE_BYTES / SECTOR_BYTES;
		}
	}

	// Read from flight at the appropriate offset (excluding metadata)
	uint32_t readAddress = firstSector * SECTOR_BYTES + FLIGHT_METADATA_PAGES * FLASH_PAGE_SIZE_BYTES + readOffset;
	S25FLX_read_start(&flash, readAddress, maxBytes, pdata);
	// If the last sector has been passed, return only the bytes that count
	if ((readAddress + maxBytes) > SECTOR_BYTES * (lastSector + 1)) {
		return (lastSector + 1) * SECTOR_BYTES - readAddress;
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
