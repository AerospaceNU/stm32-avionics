/*
 * data_log.c
 */

#include "data_log.h"

#include <stdbool.h>
#include <string.h>

#include "board_config_common.h"
#include "cli.h"

// TODO: Remove board version/ID and software version and move info to metadata.
// This method is outdated for current needs
#define BOARD_VERSION 0x01
#define BOARD_ID 0x01
#define SOFTWARE_VERSION 0x01
#define FLASH_TIMEOUT 500
#define PACKET_BUFFER_SIZE 1000
#define CONFIG_START_ADDRESS 0x200
#define MAX_LOG_PACKETS \
  FLASH_MAX_SECTOR_BYTES / sizeof(LogData_s)  // 1 sector's worth of log data

#define FLIGHT_METADATA_PAGES 2
#define FLASH_TIMEOUT_MS 500

static uint32_t flightNum = 0;
static uint32_t curSectorNum;
static uint32_t flightFirstSectorNum;
static uint32_t curWriteAddress;
static uint32_t currentConfigAddress;
static uint8_t packetBuffer[PACKET_BUFFER_SIZE];
static int curErasingFlashId;
static uint32_t logSizeBytes;

// Log Data shows what data will be logged in what relative location
typedef struct __attribute__((__packed__)) {
  uint32_t timestampS, timestampUs;
#if HAS_DEV(IMU)
  ImuDataRaw_s imuData[NUM_IMU];
#endif  // HAS_DEV(IMU)
#if HAS_DEV(ACCEL)
  Axis3dRaw_s accelData[NUM_ACCEL];
#endif  // HAS_DEV(ACCEL)
#if HAS_DEV(BAROMETER)
  BarometerData_s barometerData[NUM_BAROMETER];
#endif  // HAS_DEV(BAROMETER)
#if HAS_DEV(GPS)
  float gpsLat[NUM_GPS], gpsLong[NUM_GPS], gpsAlt[NUM_GPS];
#endif  // HAS_DEV(GPS)
#if HAS_DEV(VBAT)
  double vbatData[NUM_VBAT];
#endif  // HAS_DEV(VBAT)
#if HAS_DEV(PYRO_CONT)
  uint8_t pyroContinuity;
#endif  // HAS_DEV(PYRO_CONT)
  uint8_t triggerStatus;
  double heading, vtg;
  double pos_x, pos_y, pos_z;
  double vel_x, vel_y, vel_z;
  double acc_x, acc_y, acc_z;
  double qx, qy, qz, qw;
  uint8_t state;
} LogData_s;

static LogData_s logPackets[MAX_LOG_PACKETS];
static CircularBuffer_s logPacketBuffer;

static FlightMetadata_s FlightMetadata_sPacket;

static const uint16_t kLogData_sSize = sizeof(LogData_s);

static const uint16_t kFlightMetadata_sSize = sizeof(FlightMetadata_s);

static const uint16_t kCliConfigSize = sizeof(CliConfigs_s);

static void addressToFlashId(uint32_t startLoc, int *flashId,
                             uint32_t *flashOffset) {
  uint32_t sum = 0;
  for (int i = 0; i < NUM_FLASH; i++) {
    if (startLoc < kFlashSizeBytes[i] + sum) {
      *flashId = i;
      *flashOffset = startLoc - sum;
      return;
    }
    sum += kFlashSizeBytes[i];
  }
}

static void flash_read(uint32_t startLoc, uint32_t numBytes, uint8_t *pData) {
  int flashId = -1;
  uint32_t flashOffset = 0;
  addressToFlashId(startLoc, &flashId, &flashOffset);
  HM_FlashReadStart(flashId, flashOffset, numBytes, pData);
  uint32_t waitStartMS = HM_Millis();
  while (!HM_FlashIsReadComplete(flashId) &&
         HM_Millis() - waitStartMS < FLASH_TIMEOUT) {
  }
}

static void flash_write(uint32_t startLoc, uint32_t numBytes, uint8_t *pData) {
  int flashId = -1;
  uint32_t flashOffset = 0;
  addressToFlashId(startLoc, &flashId, &flashOffset);
  if (flashId == -1) return;

  uint32_t second_page_bytes =
      (startLoc + numBytes) % FLASH_MIN_PAGE_SIZE_BYTES;
  if (second_page_bytes >= numBytes) second_page_bytes = 0;
  uint32_t first_page_bytes = numBytes - second_page_bytes;
  HM_FlashWriteStart(flashId, flashOffset, first_page_bytes, pData);
  uint32_t waitStartMS = HM_Millis();
  while (!HM_FlashIsWriteComplete(flashId) &&
         HM_Millis() - waitStartMS < FLASH_TIMEOUT) {
  }
  if (second_page_bytes > 0) {
    addressToFlashId(startLoc + first_page_bytes, &flashId, &flashOffset);
    if (flashId == -1) return;
    HM_FlashWriteStart(flashId, flashOffset, second_page_bytes,
                       pData + first_page_bytes);
    waitStartMS = HM_Millis();
    while (!HM_FlashIsWriteComplete(flashId) &&
           HM_Millis() - waitStartMS < FLASH_TIMEOUT_MS) {
    }
  }
}

static void flash_erase_sector(uint32_t sectorNum) {
  int flashId = -1;
  uint32_t flashOffset = 0;
  addressToFlashId(sectorNum * FLASH_MAX_SECTOR_BYTES, &flashId, &flashOffset);
  if (flashId == -1) return;

  curErasingFlashId = flashId;
  HM_FlashEraseSectorStart(curErasingFlashId,
                           flashOffset / FLASH_MAX_SECTOR_BYTES);
  uint32_t waitStartMS = HM_Millis();
  while (!HM_FlashIsEraseComplete(flashId) &&
         HM_Millis() - waitStartMS < FLASH_TIMEOUT_MS) {
  }
}

void data_log_init() {
  // Sum number of bytes in full flash
  logSizeBytes = 0;
  for (int i = 0; i < NUM_FLASH; i++) {
    logSizeBytes += kFlashSizeBytes[i];
  }
}

void data_log_flash_erase(int flashId) {
  HM_FlashEraseChipStart(flashId);
  if (flashId == 0) currentConfigAddress = CONFIG_START_ADDRESS;
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
    uint8_t metadataBuff[kFlightMetadata_sSize];  // Create a buffer for the
                                                  // metadata
    uint32_t metadataReadAddress;
    while (!flightNumFound) {
      flash_read(curSectorNum * 2, 2, sectorRxBuff);
      uint16_t tempFlightNum = (sectorRxBuff[0] << 8) | sectorRxBuff[1];
      if (tempFlightNum == 0xFFFF) {
        flightNumFound = true;
      } else {
        if (launched) {  // Need to check for launched to mark as last flight
          metadataReadAddress = curSectorNum * FLASH_MAX_SECTOR_BYTES;
          flash_read(metadataReadAddress, kFlightMetadata_sSize,
                     metadataBuff);  // Read the metadata
          FlightMetadata_sPacket = *(FlightMetadata_s *)&metadataBuff;
          if (FlightMetadata_sPacket.launched ==
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
         tempCurSectorNum < logSizeBytes / FLASH_MAX_SECTOR_BYTES) {
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
         tempCurSectorNum < logSizeBytes / FLASH_MAX_SECTOR_BYTES) {
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
    *lastSector = logSizeBytes / FLASH_MAX_SECTOR_BYTES;
  }
}

void data_log_assign_flight() {
  // Initialize circular buffer of packets
  cbInit(&logPacketBuffer, logPackets, MAX_LOG_PACKETS, kLogData_sSize);

  // Assign flight number
  flightNum = data_log_get_last_flight_num() + 1;
  if (flightNum == 1) {
    // Erase sector since metadata indicates the versions are not the expected
    flash_erase_sector(0);
    // Write board version, id, and software version
    uint8_t txBuffVersions[] = {(uint16_t)(BOARD_VERSION) << 4 | BOARD_ID,
                                SOFTWARE_VERSION};
    flash_write(0, 2, txBuffVersions);
  }

  // Erase sector where flight metadata will go
  flash_erase_sector(curSectorNum);

  // Empty metadata packet
  memset(&FlightMetadata_sPacket, 0xFF, kFlightMetadata_sSize);

  // Write flight number and sector to metadata (since flight metadata written
  // to it)
  uint8_t flightTxBuff[2] = {(flightNum >> 8) & 0xFF, flightNum & 0xFF};

  // We can't have a flight past the last sector
  if (curSectorNum * FLASH_MAX_SECTOR_BYTES < logSizeBytes)
    flash_write(curSectorNum * 2, 2, flightTxBuff);

  flightFirstSectorNum = curSectorNum;
  curWriteAddress = curSectorNum * FLASH_MAX_SECTOR_BYTES +
                    FLIGHT_METADATA_PAGES * FLASH_MIN_PAGE_SIZE_BYTES;
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
  uint8_t metadataBuff[kFlightMetadata_sSize];
  uint32_t metadataReadAddress = firstSector * FLASH_MAX_SECTOR_BYTES;
  // Read the metadata
  flash_read(metadataReadAddress, kFlightMetadata_sSize, metadataBuff);
  FlightMetadata_sPacket = *(FlightMetadata_s *)metadataBuff;
}

FlightMetadata_s *data_log_get_flight_metadata() {
  return &FlightMetadata_sPacket;
}

void data_log_write_flight_metadata() {
  if (flightNum > 0) {
    uint32_t metadataWriteAddress =
        flightFirstSectorNum *
        FLASH_MAX_SECTOR_BYTES;  // Metadata is located at the start of the
                                 // flight sector
    flash_write(
        metadataWriteAddress, kFlightMetadata_sSize,
        (uint8_t *)&FlightMetadata_sPacket);  // Write the metadata packet
  }
}

void data_log_write(SensorData_s *sensorData, FilterData_s *filterData,
                    uint8_t state) {
  if (flightNum > 0) {
    // Check if current sector is beyond flash capacity. If so, stop
    if (curSectorNum >= logSizeBytes / FLASH_MAX_SECTOR_BYTES) return;

    LogData_s logPacket;
    logPacket.timestampS = sensorData->timestampS;
    logPacket.timestampUs = sensorData->timestampUs;
#if HAS_DEV(IMU)
    for (int i = 0; i < NUM_IMU; i++) {
      logPacket.imuData[i].accel = sensorData->imuData[i].accelRaw;
      logPacket.imuData[i].angVel = sensorData->imuData[i].angVelRaw;
      logPacket.imuData[i].mag = sensorData->imuData[i].magRaw;
    }
#endif  // HAS_DEV(IMU)
#if HAS_DEV(ACCEL)
    for (int i = 0; i < NUM_ACCEL; i++) {
      logPacket.accelData[i] = sensorData->accelData[i].raw;
    }
#endif  // HAS_DEV(ACCEL)
#if HAS_DEV(BAROMETER)
    for (int i = 0; i < NUM_BAROMETER; i++) {
      logPacket.barometerData[i] = sensorData->barometerData[i];
    }
#endif  // HAS_DEV(BAROMETER)
#if HAS_DEV(GPS)
    for (int i = 0; i < NUM_GPS; i++) {
      logPacket.gpsLat[i] = sensorData->gpsData[i].latitude;
      logPacket.gpsLong[i] = sensorData->gpsData[i].longitude;
      logPacket.gpsAlt[i] = sensorData->gpsData[i].altitude;
    }
#endif  // HAS_DEV(GPS)
#if HAS_DEV(VBAT)
    for (int i = 0; i < NUM_VBAT; i++) {
      logPacket.vbatData[i] = sensorData->vbatData[i];
    }
#endif  // HAS_DEV(VBAT)
#if HAS_DEV(PYRO_CONT)
    logPacket.pyroContinuity = 0;
    for (int i = 0; i < NUM_PYRO_CONT; i++) {
      logPacket.pyroContinuity |= ((sensorData->pyroContData[i] & 0x01) << i);
    }
#endif  // HAS_DEV(PYRO_CONT)
    logPacket.triggerStatus = TriggerManager_Status();
    logPacket.heading = filterData->heading;
    logPacket.vtg = filterData->vtg;
    logPacket.pos_x = filterData->pos_x;
    logPacket.pos_y = filterData->pos_y;
    logPacket.pos_z = filterData->pos_z;
    logPacket.vel_x = filterData->world_vel_x;
    logPacket.vel_y = filterData->world_vel_y;
    logPacket.vel_z = filterData->world_vel_z;
    logPacket.acc_x = filterData->world_acc_x;
    logPacket.acc_y = filterData->world_acc_y;
    logPacket.acc_z = filterData->world_acc_z;
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
      erasing = !HM_FlashIsEraseComplete(curErasingFlashId);
    } else {
      // Check if a new sector has been entered. If so, record in file system
      // and start erasing the sector
      if ((curWriteAddress + kLogData_sSize) / FLASH_MAX_SECTOR_BYTES >
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
        LogData_s dequeuedData;
        size_t numElements = 1;
        cbPeek(&logPacketBuffer, &dequeuedData, &numElements);
        while (numElements != 0) {
          cbDequeue(&logPacketBuffer, 1);
          flash_write(curWriteAddress, kLogData_sSize,
                      (uint8_t *)&dequeuedData);
          // Increment write address to be next address to write to
          curWriteAddress += kLogData_sSize;
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
  uint32_t readAddress = firstSector * FLASH_MAX_SECTOR_BYTES +
                         FLIGHT_METADATA_PAGES * FLASH_MIN_PAGE_SIZE_BYTES +
                         readOffset;
  flash_read(readAddress, maxBytes, pdata);
  // If the last sector has been passed, return only the bytes that count
  if ((readAddress + maxBytes) > FLASH_MAX_SECTOR_BYTES * (lastSector + 1)) {
    uint32_t bytesRead =
        (lastSector + 1) * FLASH_MAX_SECTOR_BYTES - readAddress;
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
  LogData_s *dataPacket;
  uint32_t firstSector;
  uint32_t lastSector;
  // Find sectors of flight num using metadata
  data_log_get_flight_sectors(flightNum, &firstSector, &lastSector);

  uint32_t firstAddress = firstSector * FLASH_MAX_SECTOR_BYTES +
                          FLIGHT_METADATA_PAGES * FLASH_MIN_PAGE_SIZE_BYTES;
  // We have this many bytes in the sectors we have
  uint32_t sectorBytes =
      (FLASH_MAX_SECTOR_BYTES * (lastSector - firstSector + 1));
  // We have to subtract the metadata from one of the sectors (first) and then
  // find number of sectors
  uint32_t maxCount =
      (sectorBytes - FLIGHT_METADATA_PAGES * FLASH_MIN_PAGE_SIZE_BYTES) /
      kLogData_sSize;
  data_log_get_last_packet_type(firstAddress, maxCount, kLogData_sSize);
  dataPacket = (LogData_s *)packetBuffer;

  return dataPacket->timestampS;
}

uint8_t data_log_get_flash_usage() {
  if (flightNum == 0) {  // In a cli state, curWriteAddress is not valid
    data_log_get_last_flight_num();
    curWriteAddress = curSectorNum * FLASH_MAX_SECTOR_BYTES +
                      FLIGHT_METADATA_PAGES * FLASH_MIN_PAGE_SIZE_BYTES;
  }
  uint8_t usage = (uint8_t)(curWriteAddress / (logSizeBytes / 100.0));
  return usage;
}

void data_log_load_cli_configs() {
  CliConfigs_s *cliConfig = cliGetConfigs();
  uint32_t firstAddress = CONFIG_START_ADDRESS;
  uint32_t maxCount =
      (FLASH_MAX_SECTOR_BYTES - CONFIG_START_ADDRESS) / kCliConfigSize;
  currentConfigAddress =
      kCliConfigSize +
      data_log_get_last_packet_type(firstAddress, maxCount, kCliConfigSize);
  *cliConfig = *(CliConfigs_s *)packetBuffer;
  if (packet_is_empty((uint8_t *)cliConfig, kCliConfigSize)) {
    cliSetDefaultConfig();
  }
#if HAS_DEV(RADIO)
  // Set the radio channel to be whatever we are using from the cli configs,
  // since we may have something saved that isn't the default that gets
  // initially set on init
  HM_RadioSetChannel(RADIO_CLI_ID, cliConfig->radioChannel);
#endif  // HAS_DEV(RADIO)
}

void data_log_write_cli_configs() {
  CliConfigs_s *cliConfig = cliGetConfigs();
  uint32_t second_page_bytes =
      (currentConfigAddress + kCliConfigSize) % FLASH_MIN_PAGE_SIZE_BYTES;
  if (second_page_bytes >= kCliConfigSize) second_page_bytes = 0;
  uint32_t first_page_bytes = kCliConfigSize - second_page_bytes;
  // Write to first page and, if necessary, second page
  flash_write(currentConfigAddress, first_page_bytes, (uint8_t *)cliConfig);
  if (second_page_bytes > 0) {
    flash_write(currentConfigAddress + first_page_bytes, second_page_bytes,
                (uint8_t *)cliConfig + first_page_bytes);
  }
  currentConfigAddress += kCliConfigSize;
  if (currentConfigAddress > FLASH_MAX_SECTOR_BYTES - kCliConfigSize) {
    flash_read(0, CONFIG_START_ADDRESS, packetBuffer);
    flash_erase_sector(0);
    flash_write(0, CONFIG_START_ADDRESS, packetBuffer);
    currentConfigAddress = CONFIG_START_ADDRESS;
  }
}
