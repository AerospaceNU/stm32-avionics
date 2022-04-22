/*
 * data_log.c
 */
#include "data_log.h"

#include <stdbool.h>
#include <string.h>

#include "cli.h"

#ifdef FCB_VERSION
const uint8_t BOARD_VERSION = FCB_VERSION;
#else
const uint8_t BOARD_VERSION = 0xff;
#endif
#define BOARD_ID 0x01
#define SOFTWARE_VERSION 0x01
#define FLASH_TIMEOUT 500
#define PACKET_BUFFER_SIZE 1000
#define CONFIG_START_ADDRESS 0x200
#define MAX_LOG_PACKETS \
  FLASH_SECTOR_BYTES / sizeof(LogData)  // 1 sector's worth of log data

static uint32_t flightNum = 0;
static uint32_t curSectorNum;
static uint32_t flightFirstSectorNum;
static uint32_t curWriteAddress;
static uint32_t currentConfigAddress;
static uint8_t packetBuffer[PACKET_BUFFER_SIZE];

// Log Data shows what data will be logged in what relative location
typedef struct __attribute__((__packed__)) LogData {
  uint32_t timestamp_s, timestamp_us;
  int16_t imu1_accel_x_raw, imu1_accel_y_raw, imu1_accel_z_raw;
  int16_t imu1_gyro_x_raw, imu1_gyro_y_raw, imu1_gyro_z_raw;
  int16_t imu1_mag_x_raw, imu1_mag_y_raw, imu1_mag_z_raw;
  int16_t imu2_accel_x_raw, imu2_accel_y_raw, imu2_accel_z_raw;
  int16_t imu2_gyro_x_raw, imu2_gyro_y_raw, imu2_gyro_z_raw;
  int16_t imu2_mag_x_raw, imu2_mag_y_raw, imu2_mag_z_raw;
  int16_t high_g_accel_x_raw, high_g_accel_y_raw, high_g_accel_z_raw;
  double baro1_temp, baro1_pres;
  double baro2_temp, baro2_pres;
  float gps_lat, gps_long, gps_alt;
  double battery_voltage;
  uint8_t pyro_continuity;
  uint8_t pyro_status;
  double heading, vtg;
  double pos_x, pos_y, pos_z;
  double vel_x, vel_y, vel_z;
  double acc_x, acc_y, acc_z;
  double qx, qy, qz, qw;
  uint8_t state;
} LogData;

static LogData logPackets[MAX_LOG_PACKETS];
static CircularBuffer_t logPacketBuffer;

static FlightMetadata flightMetadataPacket;

static const uint16_t kLogDataSize = sizeof(LogData);

static const uint16_t kFlightMetadataSize = sizeof(FlightMetadata);

static const uint16_t kCliConfigSize = sizeof(CliConfigs_t);

static void flash_read(uint32_t startLoc, uint32_t numBytes, uint8_t *pData) {
  HM_FlashReadStart(startLoc, numBytes, pData);
  uint32_t waitStartMS = HM_Millis();
  while (!HM_FlashIsReadComplete() &&
         HM_Millis() - waitStartMS < FLASH_TIMEOUT) {
  }
}

static void flash_write(uint32_t startLoc, uint32_t numBytes, uint8_t *pData) {
  if (startLoc > FLASH_SIZE_BYTES) return;
  uint32_t second_page_bytes = (startLoc + numBytes) % FLASH_PAGE_SIZE_BYTES;
  if (second_page_bytes >= numBytes) second_page_bytes = 0;
  uint32_t first_page_bytes = numBytes - second_page_bytes;
  HM_FlashWriteStart(startLoc, first_page_bytes, pData);
  uint32_t waitStartMS = HM_Millis();
  while (!HM_FlashIsWriteComplete() &&
         HM_Millis() - waitStartMS < FLASH_TIMEOUT) {
  }
  if (second_page_bytes > 0) {
    HM_FlashWriteStart(startLoc + first_page_bytes, second_page_bytes,
                       pData + first_page_bytes);
    waitStartMS = HM_Millis();
    while (!HM_FlashIsWriteComplete() &&
           HM_Millis() - waitStartMS < FLASH_TIMEOUT_MS) {
    }
  }
}

static void flash_erase_sector(uint32_t sectorNum) {
  HM_FlashEraseSectorStart(sectorNum);
  uint32_t waitStartMS = HM_Millis();
  while (!HM_FlashIsEraseComplete() &&
         HM_Millis() - waitStartMS < FLASH_TIMEOUT_MS) {
  }
}

void data_log_flash_erase() {
  HM_FlashEraseChipStart();
  currentConfigAddress = CONFIG_START_ADDRESS;
}

static uint32_t data_log_get_last_flight_num_type(bool launched) {
  uint32_t lastFlightNum = 0;
  uint8_t rxBuffVersions[2] = {0};
  flash_read(0, 2, rxBuffVersions);
  uint8_t boardVersion = (rxBuffVersions[0] >> 4) & 0x0F;
  uint8_t boardID = rxBuffVersions[0] & 0x0F;
  uint8_t softwareVersion = rxBuffVersions[1];

  if (boardVersion != BOARD_VERSION || boardID != BOARD_ID ||
      softwareVersion != SOFTWARE_VERSION) {
    lastFlightNum = 0;
    curSectorNum = 1;
  } else {
    // If board version/id/software version correct, figure out what flight it
    // is
    bool flightNumFound = false;
    curSectorNum = 1;
    uint8_t sectorRxBuff[2] = {0};
    uint8_t
        metadataBuff[kFlightMetadataSize];  // Create a buffer for the metadata
    uint32_t metadataReadAddress;
    while (!flightNumFound) {
      flash_read(curSectorNum * 2, 2, sectorRxBuff);
      uint16_t tempFlightNum = (sectorRxBuff[0] << 8) | sectorRxBuff[1];
      if (tempFlightNum == 0xFFFF) {
        flightNumFound = true;
      } else {
        if (launched) {  // Need to check for launched to mark as last flight
          metadataReadAddress = curSectorNum * FLASH_SECTOR_BYTES;
          flash_read(metadataReadAddress, kFlightMetadataSize,
                     metadataBuff);  // Read the metadata
          flightMetadataPacket = *(FlightMetadata *)&metadataBuff;
          if (flightMetadataPacket.launched ==
              1) {  // Only update last if it was launched
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

static void data_log_get_flight_sectors(uint32_t flightNum,
                                        uint32_t *firstSector,
                                        uint32_t *lastSector) {
  *firstSector = 0;
  *lastSector = 0;
  uint32_t tempCurSectorNum = 1;
  uint8_t sectorRxBuff[2] = {0};

  while (*firstSector == 0 &&
         tempCurSectorNum < FLASH_SIZE_BYTES / FLASH_SECTOR_BYTES) {
    flash_read(tempCurSectorNum * 2, 2, sectorRxBuff);
    uint16_t tempFlightNum = (sectorRxBuff[0] << 8) | sectorRxBuff[1];
    if (tempFlightNum == flightNum) {
      *firstSector = tempCurSectorNum;
    }
    tempCurSectorNum++;
  }

  // If the first sector is 0 after the while loop, the flight number doesn't
  // exist
  if (*firstSector == 0) {
    return;
  }

  // Find last sector of flight num using metadata.
  while (*lastSector == 0 &&
         tempCurSectorNum < FLASH_SIZE_BYTES / FLASH_SECTOR_BYTES) {
    flash_read(tempCurSectorNum * 2, 2, sectorRxBuff);

    uint16_t tempFlightNum = (sectorRxBuff[0] << 8) | sectorRxBuff[1];
    if (tempFlightNum != flightNum) {
      *lastSector = tempCurSectorNum - 1;
    }
    tempCurSectorNum++;
  }

  // If the last sector is still 0, the last sector of data is the last sector
  // of flash
  if (*lastSector == 0) {
    *lastSector = FLASH_SIZE_BYTES / FLASH_SECTOR_BYTES;
  }
}

void data_log_assign_flight() {
  // Initialize circular buffer of packets
  cbInit(&logPacketBuffer, logPackets, MAX_LOG_PACKETS, kLogDataSize);

  // Assign flight number
  flightNum = data_log_get_last_flight_num() + 1;
  if (flightNum == 1) {
    // Erase sector since metadata indicates the versions are not the expected
    flash_erase_sector(0);
    // Write board version, id, and software version
    uint8_t txBuffVersions[] = {BOARD_VERSION << 4 | BOARD_ID,
                                SOFTWARE_VERSION};
    flash_write(0, 2, txBuffVersions);
  }

  // Erase sector where flight metadata will go
  flash_erase_sector(curSectorNum);

  // Empty metadata packet
  memset(&flightMetadataPacket, 0xFF, kFlightMetadataSize);

  // Write flight number and sector to metadata (since flight metadata written
  // to it)
  uint8_t flightTxBuff[2] = {(flightNum >> 8) & 0xFF, flightNum & 0xFF};

  // We can't have a flight past the last sector
  if (curSectorNum * FLASH_SECTOR_BYTES < FLASH_SIZE_BYTES)
    flash_write(curSectorNum * 2, 2, flightTxBuff);

  flightFirstSectorNum = curSectorNum;
  curWriteAddress = curSectorNum * FLASH_SECTOR_BYTES +
                    FLIGHT_METADATA_PAGES * FLASH_PAGE_SIZE_BYTES;
}

void data_log_load_last_stored_flight_metadata() {
  uint32_t flightNum =
      data_log_get_last_flight_num();  // Load the previous flight number
  data_log_read_flight_num_metadata(flightNum);
}

void data_log_read_flight_num_metadata(uint8_t flightNum) {
  uint32_t firstSector, lastSector;
  data_log_get_flight_sectors(flightNum, &firstSector, &lastSector);
  // Create a buffer for the metadata
  uint8_t metadataBuff[kFlightMetadataSize];
  uint32_t metadataReadAddress = firstSector * FLASH_SECTOR_BYTES;
  // Read the metadata
  flash_read(metadataReadAddress, kFlightMetadataSize, metadataBuff);
  flightMetadataPacket = *(FlightMetadata *)metadataBuff;
}

FlightMetadata *data_log_get_flight_metadata() { return &flightMetadataPacket; }

void data_log_write_flight_metadata() {
  if (flightNum > 0) {
    uint32_t metadataWriteAddress =
        flightFirstSectorNum *
        FLASH_SECTOR_BYTES;  // Metadata is located at the start of the flight
                             // sector
    flash_write(metadataWriteAddress, kFlightMetadataSize,
                (uint8_t *)&flightMetadataPacket);  // Write the metadata packet
  }
}

void data_log_write(SensorData_t *sensorData, FilterData_t *filterData,
                    uint8_t state) {
  if (flightNum > 0) {
    // Check if current sector is beyond flash capacity. If so, stop
    if (curSectorNum >= FLASH_SIZE_BYTES / FLASH_SECTOR_BYTES) return;

    LogData logPacket;
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
      logPacket.pyro_continuity |=
          ((sensorData->pyro_continuity[i] & 0x01) << i);
    }
    logPacket.pyro_status = PyroManager_Status();
    logPacket.heading = filterData->heading;
    logPacket.vtg = filterData->vtg;
    logPacket.pos_x = filterData->pos_x;
    logPacket.pos_y = filterData->pos_y;
    logPacket.pos_z = filterData->pos_z;
    logPacket.vel_x = filterData->vel_x;
    logPacket.vel_y = filterData->vel_y;
    logPacket.vel_z = filterData->vel_z;
    logPacket.acc_x = filterData->acc_x;
    logPacket.acc_y = filterData->acc_y;
    logPacket.acc_z = filterData->acc_z;
    logPacket.qx = filterData->qx;
    logPacket.qy = filterData->qy;
    logPacket.qz = filterData->qz;
    logPacket.qw = filterData->qw;
    logPacket.state = state;

    // Add newest log packet to buffer, overriding oldest packet
    if (cbFull(&logPacketBuffer)) {
      cbDequeue(&logPacketBuffer, 1);
    }
    cbEnqueue(&logPacketBuffer, &logPacket);

    static bool erasing = false;
    if (erasing) {
      // If currently erasing, check if erase is complete
      erasing = !HM_FlashIsEraseComplete();
    } else {
      // Check if a new sector has been entered. If so, record in file system
      // and start erasing the sector
      if ((curWriteAddress + kLogDataSize) / FLASH_SECTOR_BYTES >
          curSectorNum) {
        curSectorNum++;
        // Write flight number and sector to metadata
        uint8_t flightTxBuff[2] = {(flightNum >> 8) & 0xFF, flightNum & 0xFF};
        flash_write(curSectorNum * 2, 2, flightTxBuff);
        // Start erasing the upcoming sector
        flash_erase_sector(curSectorNum);
        erasing = true;
      } else {
        // Write whole log buffer. No need to worry about erasing future
        // sector since log buffer is only 1 sector long.
        LogData dequeuedData;
        size_t numElements = 1;
        cbPeek(&logPacketBuffer, &dequeuedData, &numElements);
        while (numElements != 0) {
          cbDequeue(&logPacketBuffer, 1);
          flash_write(curWriteAddress, kLogDataSize, (uint8_t *)&dequeuedData);
          // Increment write address to be next address to write to
          curWriteAddress += kLogDataSize;
          cbPeek(&logPacketBuffer, &dequeuedData, &numElements);
        }
      }
    }
  }
}

uint32_t data_log_read(uint32_t flightNum, uint32_t maxBytes, uint8_t *pdata,
                       bool reset) {
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
  uint32_t readAddress = firstSector * FLASH_SECTOR_BYTES +
                         FLIGHT_METADATA_PAGES * FLASH_PAGE_SIZE_BYTES +
                         readOffset;
  flash_read(readAddress, maxBytes, pdata);
  // If the last sector has been passed, return only the bytes that count
  if ((readAddress + maxBytes) > FLASH_SECTOR_BYTES * (lastSector + 1)) {
    uint32_t bytesRead = (lastSector + 1) * FLASH_SECTOR_BYTES - readAddress;
    readOffset += bytesRead;
    return bytesRead;
  }
  // If all bytes are 0xFF, return that none count since that part of the log
  // wasn't written to Only apply this if the max read bytes is large enough.
  // Otherwise, the 0xFF could be caused by the data
  if (maxBytes > 20) {
    for (int i = 0; i < maxBytes; i++) {
      if (*(pdata + i) != 0xFF) break;
      if (i == maxBytes - 1) {
        return 0;
      }
    }
  }

  readOffset += maxBytes;
  return maxBytes;
}

static bool packet_is_empty(uint8_t *buffer, uint16_t packetSize) {
  for (int i = 0; i < packetSize; ++i) {
    if (buffer[i] != 0xFF) return false;
  }
  return true;
}

static uint32_t data_log_get_last_packet_type(uint32_t firstAddress,
                                              uint32_t maxCount,
                                              uint16_t packetSize) {
  // Binary search to find the last packet
  uint32_t low = 0;
  uint32_t high = maxCount;
  uint32_t mid = (high + low) / 2;
  uint32_t pReadAddress = firstAddress + mid * packetSize;

  while (low < mid) {
    pReadAddress = firstAddress + mid * packetSize;
    flash_read(pReadAddress, packetSize, packetBuffer);
    // All FFs = too high
    if (packet_is_empty(packetBuffer, packetSize)) {
      high = mid;
      mid = (high + low) / 2;
    } else {
      low = mid;
      mid = (high + low) / 2;
    }
  }
  pReadAddress = firstAddress + mid * packetSize;
  flash_read(pReadAddress, packetSize, packetBuffer);
  return pReadAddress;
}

uint32_t data_log_get_last_flight_timestamp(uint32_t flightNum) {
  LogData *dataPacket;
  uint32_t firstSector;
  uint32_t lastSector;
  // Find sectors of flight num using metadata
  data_log_get_flight_sectors(flightNum, &firstSector, &lastSector);

  uint32_t firstAddress = firstSector * FLASH_SECTOR_BYTES +
                          FLIGHT_METADATA_PAGES * FLASH_PAGE_SIZE_BYTES;
  // We have this many bytes in the sectors we have
  uint32_t sectorBytes = (FLASH_SECTOR_BYTES * (lastSector - firstSector + 1));
  // We have to subtract the metadata from one of the sectors (first) and then
  // find number of sectors
  uint32_t maxCount =
      (sectorBytes - FLIGHT_METADATA_PAGES * FLASH_PAGE_SIZE_BYTES) /
      kLogDataSize;
  data_log_get_last_packet_type(firstAddress, maxCount, kLogDataSize);
  dataPacket = (LogData *)packetBuffer;

  return dataPacket->timestamp_s;
}

uint8_t data_log_get_flash_usage() {
  if (flightNum == 0) {  // In a cli state, curWriteAddress is not valid
    data_log_get_last_flight_num();
    curWriteAddress = curSectorNum * FLASH_SECTOR_BYTES +
                      FLIGHT_METADATA_PAGES * FLASH_PAGE_SIZE_BYTES;
  }
  uint8_t usage = (uint8_t)(curWriteAddress / (FLASH_SIZE_BYTES / 100.0));
  return usage;
}

void data_log_load_cli_configs() {
  CliConfigs_t *cliConfig = cliGetConfigs();
  uint32_t firstAddress = CONFIG_START_ADDRESS;
  uint32_t maxCount =
      (FLASH_SECTOR_BYTES - CONFIG_START_ADDRESS) / kCliConfigSize;
  currentConfigAddress =
      kCliConfigSize +
      data_log_get_last_packet_type(firstAddress, maxCount, kCliConfigSize);
  *cliConfig = *(CliConfigs_t *)packetBuffer;
  if (packet_is_empty((uint8_t *)cliConfig, kCliConfigSize)) {
    cliSetDefaultConfig();
  }
#ifdef TELEMETRY_RADIO
  HM_RadioSetChannel(TELEMETRY_RADIO, cliConfig->radioChannel);
#endif
}

void data_log_write_cli_configs() {
  CliConfigs_t *cliConfig = cliGetConfigs();
  uint32_t second_page_bytes =
      (currentConfigAddress + kCliConfigSize) % FLASH_PAGE_SIZE_BYTES;
  if (second_page_bytes >= kCliConfigSize) second_page_bytes = 0;
  uint32_t first_page_bytes = kCliConfigSize - second_page_bytes;
  // Write to first page and, if necessary, second page
  flash_write(currentConfigAddress, first_page_bytes, (uint8_t *)cliConfig);
  if (second_page_bytes > 0) {
    flash_write(currentConfigAddress + first_page_bytes, second_page_bytes,
                (uint8_t *)cliConfig + first_page_bytes);
  }
  currentConfigAddress += kCliConfigSize;
  if (currentConfigAddress > FLASH_SECTOR_BYTES - kCliConfigSize) {
    flash_read(0, CONFIG_START_ADDRESS, packetBuffer);
    flash_erase_sector(0);
    flash_write(0, CONFIG_START_ADDRESS, packetBuffer);
    currentConfigAddress = CONFIG_START_ADDRESS;
  }
}
