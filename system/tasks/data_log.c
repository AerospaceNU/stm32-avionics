/*
 * data_log.c
 */

#include "data_log.h"

#include <stdbool.h>
#include <string.h>

#include "board_config_common.h"
#include "cli.h"

#define PACKET_BUFFER_SIZE 1000
#define CONFIG_START_ADDRESS 0x200
#define MAX_LOG_PACKETS \
  FLASH_MAX_SECTOR_BYTES / sizeof(LogData_s)  // 1 sector's worth of log data

#define FLIGHT_METADATA_PAGES 2
#define FLASH_TIMEOUT_MS 500

static uint32_t curFlightNum;
static uint32_t curSectorNum;
static uint32_t curWriteAddress;
static uint32_t currentConfigAddress;
static uint32_t curFlightFirstSectorNum;
static int curErasingFlashId;
static uint32_t logSizeBytes;
#if HAS_DEV(LINE_CUTTER_BLE)
static uint32_t lineCutterLastLogTime[NUM_LINE_CUTTER_BLE] = {0};
#endif  // HAS_DEV(LINE_CUTTER_BLE)

static uint8_t tempPacketBuffer[PACKET_BUFFER_SIZE];

#define LOG_ID_FCB 0

typedef struct __attribute__((__packed__)) {
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
  GpsGeneralData_s gpsData[NUM_GPS];
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
} FcbLogData_s;

#define LOG_ID_LINE_CUTTER 1

typedef union {
  FcbLogData_s fcbData;
  LineCutterData_s lineCutterData;
} LogDataPacket_s;

typedef struct __attribute__((__packed__)) {
  uint8_t packetType;
  uint32_t timestampMs;
  LogDataPacket_s dataPacket;
} LogData_s;

static LogData_s logPackets[MAX_LOG_PACKETS];
static CircularBuffer_s logPacketBuffer;

static FlightMetadata_s flightMetadataPacket;

static const uint16_t kLogDataSize = sizeof(LogData_s);

static const uint16_t kFlightMetadataSize = sizeof(FlightMetadata_s);

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

void dataLog_flashRead(uint32_t startLoc, uint32_t numBytes, uint8_t *pData) {
  int flashId = -1;
  uint32_t flashOffset = 0;
  addressToFlashId(startLoc, &flashId, &flashOffset);
  hm_flashReadStart(flashId, flashOffset, numBytes, pData);
  uint32_t waitStartMS = hm_millis();
  while (!hm_flashIsReadComplete(flashId) &&
         hm_millis() - waitStartMS < FLASH_TIMEOUT_MS) {
  }
}

static void flashWrite(uint32_t startLoc, uint32_t numBytes, uint8_t *pData) {
  int flashId = -1;
  uint32_t flashOffset = 0;
  addressToFlashId(startLoc, &flashId, &flashOffset);
  if (flashId == -1) return;

  uint32_t second_page_bytes =
      (startLoc + numBytes) % FLASH_MIN_PAGE_SIZE_BYTES;
  if (second_page_bytes >= numBytes) second_page_bytes = 0;
  uint32_t first_page_bytes = numBytes - second_page_bytes;
  hm_flashWriteStart(flashId, flashOffset, first_page_bytes, pData);
  uint32_t waitStartMS = hm_millis();
  while (!hm_flashIsWriteComplete(flashId) &&
         hm_millis() - waitStartMS < FLASH_TIMEOUT_MS) {
  }
  if (second_page_bytes > 0) {
    addressToFlashId(startLoc + first_page_bytes, &flashId, &flashOffset);
    if (flashId == -1) return;
    hm_flashWriteStart(flashId, flashOffset, second_page_bytes,
                       pData + first_page_bytes);
    waitStartMS = hm_millis();
    while (!hm_flashIsWriteComplete(flashId) &&
           hm_millis() - waitStartMS < FLASH_TIMEOUT_MS) {
    }
  }
}

static void flashEraseSector(uint32_t sectorNum) {
  int flashId = -1;
  uint32_t flashOffset = 0;
  addressToFlashId(sectorNum * FLASH_MAX_SECTOR_BYTES, &flashId, &flashOffset);
  if (flashId == -1) return;

  curErasingFlashId = flashId;
  hm_flashEraseSectorStart(curErasingFlashId,
                           flashOffset / FLASH_MAX_SECTOR_BYTES);
  uint32_t waitStartMS = hm_millis();
  while (!hm_flashIsEraseComplete(flashId) &&
         hm_millis() - waitStartMS < FLASH_TIMEOUT_MS) {
  }
}

void dataLog_init() {
  // Sum number of bytes in full flash
  logSizeBytes = 0;
  for (int i = 0; i < NUM_FLASH; i++) {
    logSizeBytes += kFlashSizeBytes[i];
  }
}

void dataLog_flashErase(int flashId) {
  hm_flashEraseChipStart(flashId);
  if (flashId == 0) currentConfigAddress = CONFIG_START_ADDRESS;
}

static uint32_t dataLog_getLastFlightNumType(bool launched) {
  uint32_t lastFlightNum = 0, mostRecentFlightNum = 0;

  // Figure out what flight it is
  bool flightNumFound = false;
  uint32_t searchSectorNum = 1;
  uint8_t sectorRxBuff[2] = {0};
  uint8_t metadataBuff[kFlightMetadataSize];  // Create a buffer for the
                                              // metadata
  uint32_t metadataReadAddress;
  while (!flightNumFound) {
    dataLog_flashRead(searchSectorNum * 2, 2, sectorRxBuff);
    uint16_t tempFlightNum = (sectorRxBuff[0] << 8) | sectorRxBuff[1];
    if (tempFlightNum == 0xFFFF) {
      flightNumFound = true;
    } else {
      if (launched && (mostRecentFlightNum !=
                       tempFlightNum)) {  // Need to check for launched to mark
                                          // as last flight
        metadataReadAddress = searchSectorNum * FLASH_MAX_SECTOR_BYTES;
        dataLog_flashRead(metadataReadAddress, kFlightMetadataSize,
                  metadataBuff);  // Read the metadata
        FlightMetadata_s tempFlightMetadataPacket =
            *(FlightMetadata_s *)metadataBuff;
        if (tempFlightMetadataPacket.launched ==
            1) {  // Only update last if it was launched
          lastFlightNum = tempFlightNum;
        }
      } else {
        lastFlightNum = tempFlightNum;
      }
      searchSectorNum++;
    }
    mostRecentFlightNum = tempFlightNum;
  }

  return lastFlightNum;
}

uint32_t dataLog_getLastFlightNum() {
  return dataLog_getLastFlightNumType(false);
}

uint32_t dataLog_getLastLaunchedFlightNum() {
  return dataLog_getLastFlightNumType(true);
}

void dataLog_getFlightSectors(uint32_t flightNum, uint32_t *firstSector,
                                     uint32_t *lastSector) {
  *firstSector = 0;
  *lastSector = 0;
  uint32_t tempCurSectorNum = 1;
  uint8_t sectorRxBuff[2] = {0};

  while (*firstSector == 0 &&
         tempCurSectorNum < logSizeBytes / FLASH_MAX_SECTOR_BYTES) {
    dataLog_flashRead(tempCurSectorNum * 2, 2, sectorRxBuff);
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
    dataLog_flashRead(tempCurSectorNum * 2, 2, sectorRxBuff);

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

void dataLog_assignFlight() {
  // Initialize circular buffer of packets
  cb_init(&logPacketBuffer, logPackets, MAX_LOG_PACKETS, kLogDataSize);

  // Assign flight number
  curFlightNum = dataLog_getLastFlightNum() + 1;
  if (curFlightNum == 1) {
    // Erase first sector since we don't know what's inside
    flashEraseSector(0);
    curSectorNum = 1;
  } else {
    uint32_t unused;
    dataLog_getFlightSectors(curFlightNum - 1, &unused, &curSectorNum);
    curSectorNum += 1;  // 1 beyond last flight sector
  }

  // Erase sector where flight metadata will go
  flashEraseSector(curSectorNum);

  // Empty metadata packet
  memset(&flightMetadataPacket, 0xFF, kFlightMetadataSize);

  // Write flight number and sector to metadata (since flight metadata written
  // to it)
  uint8_t flightTxBuff[2] = {(curFlightNum >> 8) & 0xFF, curFlightNum & 0xFF};

  // We can't have a flight past the last sector
  if (curSectorNum * FLASH_MAX_SECTOR_BYTES < logSizeBytes)
    flashWrite(curSectorNum * 2, 2, flightTxBuff);

  curFlightFirstSectorNum = curSectorNum;
  curWriteAddress = curSectorNum * FLASH_MAX_SECTOR_BYTES +
                    FLIGHT_METADATA_PAGES * FLASH_MIN_PAGE_SIZE_BYTES;
}

void dataLog_loadLastStoredFlightMetadata() {
  uint32_t flightNum =
      dataLog_getLastFlightNum();  // Load the previous flight number
  dataLog_readFlightNumMetadata(flightNum);
}

void dataLog_readFlightNumMetadata(uint8_t flightNum) {
  uint32_t firstSector, lastSector;
  dataLog_getFlightSectors(flightNum, &firstSector, &lastSector);
  // Create a buffer for the metadata
  uint8_t metadataBuff[kFlightMetadataSize];
  uint32_t metadataReadAddress = firstSector * FLASH_MAX_SECTOR_BYTES;
  // Read the metadata
  dataLog_flashRead(metadataReadAddress, kFlightMetadataSize, metadataBuff);
  flightMetadataPacket = *(FlightMetadata_s *)metadataBuff;
}

FlightMetadata_s *dataLog_getFlightMetadata() { return &flightMetadataPacket; }

void dataLog_writeFlightMetadata() {
  if (curFlightNum > 0) {
    uint32_t metadataWriteAddress =
        curFlightFirstSectorNum *
        FLASH_MAX_SECTOR_BYTES;  // Metadata is located at the start of the
                                 // flight sector
    flashWrite(metadataWriteAddress, kFlightMetadataSize,
               (uint8_t *)&flightMetadataPacket);  // Write the metadata packet
  }
}

void dataLog_write(SensorData_s *sensorData, FilterData_s *filterData,
                   uint8_t state) {
  if (curFlightNum > 0) {
    // Check if current sector is beyond flash capacity. If so, stop
    if (curSectorNum >= logSizeBytes / FLASH_MAX_SECTOR_BYTES) return;

    LogData_s logPacket;
    logPacket.packetType = LOG_ID_FCB;
    logPacket.timestampMs = sensorData->timestampMs;
    FcbLogData_s *fcbLogData = &(logPacket.dataPacket.fcbData);
#if HAS_DEV(IMU)
    for (int i = 0; i < NUM_IMU; i++) {
      fcbLogData->imuData[i].accel = sensorData->imuData[i].accelRaw;
      fcbLogData->imuData[i].angVel = sensorData->imuData[i].angVelRaw;
      fcbLogData->imuData[i].mag = sensorData->imuData[i].magRaw;
    }
#endif  // HAS_DEV(IMU)
#if HAS_DEV(ACCEL)
    for (int i = 0; i < NUM_ACCEL; i++) {
      fcbLogData->accelData[i] = sensorData->accelData[i].raw;
    }
#endif  // HAS_DEV(ACCEL)
#if HAS_DEV(BAROMETER)
    for (int i = 0; i < NUM_BAROMETER; i++) {
      fcbLogData->barometerData[i] = sensorData->barometerData[i];
    }
#endif  // HAS_DEV(BAROMETER)
#if HAS_DEV(GPS)
    for (int i = 0; i < NUM_GPS; i++) {
      fcbLogData->gpsData[i] = sensorData->gpsData[i].generalData;
    }
#endif  // HAS_DEV(GPS)
#if HAS_DEV(VBAT)
    for (int i = 0; i < NUM_VBAT; i++) {
      fcbLogData->vbatData[i] = sensorData->vbatData[i];
    }
#endif  // HAS_DEV(VBAT)
#if HAS_DEV(PYRO_CONT)
    fcbLogData->pyroContinuity = 0;
    for (int i = 0; i < NUM_PYRO_CONT; i++) {
      fcbLogData->pyroContinuity |= ((sensorData->pyroContData[i] & 0x01) << i);
    }
#endif  // HAS_DEV(PYRO_CONT)
    fcbLogData->triggerStatus = triggerManager_status();
    fcbLogData->heading = filterData->heading;
    fcbLogData->vtg = filterData->vtg;
    fcbLogData->pos_x = filterData->pos_x;
    fcbLogData->pos_y = filterData->pos_y;
    fcbLogData->pos_z = filterData->pos_z_agl;
    fcbLogData->vel_x = filterData->world_vel_x;
    fcbLogData->vel_y = filterData->world_vel_y;
    fcbLogData->vel_z = filterData->world_vel_z;
    fcbLogData->acc_x = filterData->world_acc_x;
    fcbLogData->acc_y = filterData->world_acc_y;
    fcbLogData->acc_z = filterData->world_acc_z;
    fcbLogData->qx = filterData->qx;
    fcbLogData->qy = filterData->qy;
    fcbLogData->qz = filterData->qz;
    fcbLogData->qw = filterData->qw;
    fcbLogData->state = state;

    // Add newest log packet to buffer, overriding oldest packet
    if (cb_full(&logPacketBuffer)) {
      cb_dequeue(&logPacketBuffer, 1);
    }
    cb_enqueue(&logPacketBuffer, &logPacket);

#if HAS_DEV(LINE_CUTTER_BLE)
    for (int i = 0; i < NUM_LINE_CUTTER_BLE; ++i) {
      if (lineCutterLastLogTime[i] < hm_getLineCutterData(i)->timestamp) {
        logPacket.packetType = LOG_ID_LINE_CUTTER;
        logPacket.dataPacket.lineCutterData = *hm_getLineCutterData(i);
        lineCutterLastLogTime[i] = hm_getLineCutterData(i)->timestamp;
        // Line cutter data is less important so don't dequeue other data if it
        // can't fit
        if (!cb_full(&logPacketBuffer)) {
          cb_enqueue(&logPacketBuffer, &logPacket);
        }
      }
    }
#endif

    static bool erasing = false;
    if (erasing) {
      // If currently erasing, check if erase is complete
      erasing = !hm_flashIsEraseComplete(curErasingFlashId);
    } else {
      // Check if a new sector has been entered. If so, record in file system
      // and start erasing the sector
      if ((curWriteAddress + kLogDataSize) / FLASH_MAX_SECTOR_BYTES >
          curSectorNum) {
        curSectorNum++;
        // Write flight number and sector to metadata
        uint8_t flightTxBuff[2] = {(curFlightNum >> 8) & 0xFF,
                                   curFlightNum & 0xFF};
        flashWrite(curSectorNum * 2, 2, flightTxBuff);
        // Start erasing the upcoming sector
        flashEraseSector(curSectorNum);
        erasing = true;
      } else {
        // Write whole log buffer. No need to worry about erasing future
        // sector since log buffer is only 1 sector long.
        LogData_s dequeuedData;
        size_t numElements = 1;
        cb_peek(&logPacketBuffer, &dequeuedData, &numElements);
        while (numElements != 0) {
          cb_dequeue(&logPacketBuffer, 1);
          flashWrite(curWriteAddress, kLogDataSize, (uint8_t *)&dequeuedData);
          // Increment write address to be next address to write to
          curWriteAddress += kLogDataSize;
          cb_peek(&logPacketBuffer, &dequeuedData, &numElements);
        }
      }
    }
  }
}

uint32_t dataLog_read(uint32_t flightNum, uint32_t maxBytes, uint8_t *pdata,
                      bool reset) {
  static uint32_t firstSector;
  static uint32_t lastSector;
  static uint32_t lastFlightNum;
  static uint32_t readOffset;
  // Find sectors of flight num using metadata if no offset specified
  if ((flightNum != lastFlightNum) || reset) {
    lastFlightNum = flightNum;
    readOffset = 0;
    dataLog_getFlightSectors(flightNum, &firstSector, &lastSector);
  }

  // Read from flight at the appropriate offset (excluding metadata)
  uint32_t readAddress = firstSector * FLASH_MAX_SECTOR_BYTES +
                         FLIGHT_METADATA_PAGES * FLASH_MIN_PAGE_SIZE_BYTES +
                         readOffset;
  dataLog_flashRead(readAddress, maxBytes, pdata);
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

static bool packetIsEmpty(uint8_t *buffer, uint16_t packetSize) {
  for (int i = 0; i < packetSize; ++i) {
    if (buffer[i] != 0xFF) return false;
  }
  return true;
}

static uint32_t dataLog_getLastPacketType(uint32_t firstAddress,
                                          uint32_t maxCount,
                                          uint16_t packetSize) {
  // Binary search to find the last packet
  uint32_t low = 0;
  uint32_t high = maxCount;
  uint32_t mid = (high + low) / 2;
  uint32_t pReadAddress = firstAddress + mid * packetSize;

  while (low < mid) {
    pReadAddress = firstAddress + mid * packetSize;
    dataLog_flashRead(pReadAddress, packetSize, tempPacketBuffer);
    // All FFs = too high
    if (packetIsEmpty(tempPacketBuffer, packetSize)) {
      high = mid;
      mid = (high + low) / 2;
    } else {
      low = mid;
      mid = (high + low) / 2;
    }
  }
  pReadAddress = firstAddress + mid * packetSize;
  dataLog_flashRead(pReadAddress, packetSize, tempPacketBuffer);
  return pReadAddress;
}

uint32_t dataLog_getLastFlightTimestamp(uint32_t flightNum) {
  LogData_s *dataPacket;
  uint32_t firstSector;
  uint32_t lastSector;
  // Find sectors of flight num using metadata
  dataLog_getFlightSectors(flightNum, &firstSector, &lastSector);

  uint32_t firstAddress = firstSector * FLASH_MAX_SECTOR_BYTES +
                          FLIGHT_METADATA_PAGES * FLASH_MIN_PAGE_SIZE_BYTES;
  // We have this many bytes in the sectors we have
  uint32_t sectorBytes =
      (FLASH_MAX_SECTOR_BYTES * (lastSector - firstSector + 1));
  // We have to subtract the metadata from one of the sectors (first) and then
  // find number of sectors
  uint32_t maxCount =
      (sectorBytes - FLIGHT_METADATA_PAGES * FLASH_MIN_PAGE_SIZE_BYTES) /
      kLogDataSize;
  dataLog_getLastPacketType(firstAddress, maxCount, kLogDataSize);
  dataPacket = (LogData_s *)tempPacketBuffer;

  return dataPacket->timestampMs;
}

uint8_t dataLog_getFlashUsage() {
  uint8_t usage = (uint8_t)(curWriteAddress / (logSizeBytes / 100.0));
  return usage;
}

void dataLog_loadCliConfigs() {
  CliConfigs_s *cliConfig = cli_getConfigs();
  uint32_t firstAddress = CONFIG_START_ADDRESS;
  uint32_t maxCount =
      (FLASH_MAX_SECTOR_BYTES - CONFIG_START_ADDRESS) / kCliConfigSize;
  currentConfigAddress =
      kCliConfigSize +
      dataLog_getLastPacketType(firstAddress, maxCount, kCliConfigSize);
  *cliConfig = *(CliConfigs_s *)tempPacketBuffer;
  if (packetIsEmpty((uint8_t *)cliConfig, kCliConfigSize)) {
    currentConfigAddress -= kCliConfigSize;
    cli_setDefaultConfig();
  }
#if HAS_DEV(RADIO)
  // Set the radio channel to be whatever we are using from the cli configs,
  // since we may have something saved that isn't the default that gets
  // initially set on init
  hm_radioSetChannel(RADIO_CLI_ID, cliConfig->radioChannel);
#endif  // HAS_DEV(RADIO)
}

void dataLog_writeCliConfigs() {
  CliConfigs_s *cliConfig = cli_getConfigs();
  uint32_t second_page_bytes =
      (currentConfigAddress + kCliConfigSize) % FLASH_MIN_PAGE_SIZE_BYTES;
  if (second_page_bytes >= kCliConfigSize) second_page_bytes = 0;
  uint32_t first_page_bytes = kCliConfigSize - second_page_bytes;
  // Write to first page and, if necessary, second page
  flashWrite(currentConfigAddress, first_page_bytes, (uint8_t *)cliConfig);
  if (second_page_bytes > 0) {
    flashWrite(currentConfigAddress + first_page_bytes, second_page_bytes,
               (uint8_t *)cliConfig + first_page_bytes);
  }
  currentConfigAddress += kCliConfigSize;
  if (currentConfigAddress > FLASH_MAX_SECTOR_BYTES - kCliConfigSize) {
    dataLog_flashRead(0, CONFIG_START_ADDRESS, tempPacketBuffer);
    flashEraseSector(0);
    flashWrite(0, CONFIG_START_ADDRESS, tempPacketBuffer);
    currentConfigAddress = CONFIG_START_ADDRESS;
  }
}
