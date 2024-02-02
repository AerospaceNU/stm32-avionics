/*
 * radio_manager.c
 */

#include "radio_manager.h"

#include <stdio.h>
#include <string.h>

#include <algorithm>

#include "board_config_common.h"
#include "cli.h"
#include "data_log.h"
#include "data_structures.h"
#include "hardware_manager.h"
#include "packet_encoder.h"

#define RADIO_INTERVAL_MS 200
#define RADIO_SEND_MS 100

static const char *call = "KM6GNL";

// https://stackoverflow.com/q/9695329
#define ROUND_2_INT(f) ((int)((f) >= 0.0 ? (f + 0.5) : (f - 0.5)))

// this is gross and smelly, but for rn, its valid that one board has only one
// kind of radio on it. In the future, refactor based on transmit mode to
// support lora as well
#if HAS_DEV(RADIO_TI_433) || HAS_DEV(RADIO_TI_915)
FSKPacketRadioEncoder packetEncoder;
#else
PassthroughRadioEncoder packetEncoder;
#endif

/**
 * calculate the CRC of a packet, up to but excluding the CRC field at the end
 */
static uint16_t calculateRadioPacketCRC(RadioDecodedPacket_s &packet) {
  // Init value for CRC calculation, by convention
  uint16_t checksum = 0xFFFF;

  // iterate over whole packet, up until CRC
  uint8_t *pktAsBytes = reinterpret_cast<uint8_t *>(&packet);
  for (int i = 0; i < sizeof(packet) - sizeof(packet.packetCRC); i++) {
    checksum = ti_fec::calculateCRC(pktAsBytes[i], checksum);
  }

  return checksum;
}

void RadioManager::init(int id) {
  radioId = id;

  for (int i = 0; i < NUM_RADIO; i++) {
    cb_init(&rxBuffer, rxArray, RX_BUFF_LEN, sizeof(RadioRecievedOTAPacket));
    hm_radioRegisterConsumer(i, &rxBuffer);

    strncpy(transmitPacket.callsign, call, 8);

#ifdef SOFTWARE_VERSION
    transmitPacket.softwareVersion = SOFTWARE_VERSION;  // TODO set this;
#endif
    transmitPacket.board_serial_num = 0;  // TODO set this;

    hm_radioSetChannel(i, 2);
  }
}

//! Must be called periodically to output data over CLI or USB
void RadioManager::tick() {
  static RadioRecievedOTAPacket packet;

  // Try to dequeue all the packets we've gotten
  static size_t len = sizeof(packet);
  while (cb_count(&rxBuffer)) {
    cb_peek(&rxBuffer, (unknownPtr_t)&packet, &len);
    if (len) {
      // first decode the packet
      RadioDecodedRecievedPacket_s decoded;
      decoded.metadata = packet.metadata;
      if (0 == packetEncoder.Decode(packet.payload, decoded.payload)) {
        // CRC of bytes we got from the radio
        uint16_t localCRC = calculateRadioPacketCRC(decoded.payload);
        // The CRC set by the sender
        uint16_t remoteCRC = decoded.payload.packetCRC;
        // and compare
        decoded.decodeMetadata.crcGood = (localCRC == remoteCRC);

        // Send to all our callbacks
        for (size_t j = 0; j < numCallbacks; j++) {
          if (callbacks[j]) callbacks[j](&decoded);
        }
      } else {
        // Failed to decode?? give up is the best we can do?
        printf("packet decode failed????\n");
      }

      cb_dequeue(&rxBuffer, 1);
    } else {
      cb_flush(&rxBuffer);
    }
  }
}

void RadioManager::sendInternal(RadioDecodedPacket_s &packet) {
  // set the CRC
  packet.packetCRC = calculateRadioPacketCRC(packet);

  // and encode
  static RadioOTAPayload_s output;
  if (0 == packetEncoder.Encode(packet, output)) {
    hm_radioSend(radioId, output.payload, output.payloadLen);
  } else {
    printf("Radio message encode failed?\n");
  }
}

void RadioManager::addMessageCallback(RadioCallback_t callback) {
  callbacks[numCallbacks] = callback;
  numCallbacks++;
}

void RadioManager::transmitData(SensorData_s *sensorData,
                                FilterData_s *filterData, uint8_t state) {
  uint32_t currentTime = hm_millis();

  if (currentTime % RADIO_INTERVAL_MS >= RADIO_SEND_MS) return;

  transmitPacket.timestampMs = currentTime;

  if (timer.orientationTimer.Expired(currentTime)) {
    timer.orientationTimer.Reset();

    OrientationPacket_s data = {
      state,
      (int8_t)(filterData->qw * 100.0),
      (int8_t)(filterData->qx * 100.0),
      (int8_t)(filterData->qy * 100.0),
      (int8_t)(filterData->qz * 100.0),
      (float)filterData->rocket_ang_vel_x,
      (float)filterData->rocket_ang_vel_y,
      (float)filterData->rocket_ang_vel_z,
      (float)filterData->world_acc_x,
      (float)filterData->world_acc_y,
      (float)filterData->world_acc_z,
#if HAS_DEV(MAG)
      (float)sensorData->magData[0].realGauss.x,
      (float)sensorData->magData[0].realGauss.y,
      (float)sensorData->magData[0].realGauss.z,
#else
      0,
      0,
      0,
#endif  // HAS_DEV(IMU)

      // 16 bit means 16,000 max
      // we can just multiply by 10 for 0.1 precision
      // and improve more if required later
      static_cast<int16_t>(ROUND_2_INT(filterData->angle_vertical * 10))
    };
    transmitPacket.packetType = 2;
    transmitPacket.payload.orientation = data;

    sendInternal(transmitPacket);
  }

  if (timer.positionTimer.Expired(currentTime)) {
    timer.positionTimer.Reset();
    PositionPacket_s data = {
#if HAS_DEV(BAROMETER)
      (float)sensorData->barometerData[0].temperatureC,
#else
      0,
#endif  // HAS_DEV(BAROMETER)
      (float)filterData->pos_z_agl,
      (float)filterData->world_vel_z,
#if HAS_DEV(GPS)
      sensorData->gpsData[0].generalData.latitude,
      sensorData->gpsData[0].generalData.longitude,
      sensorData->gpsData[0].generalData.altitude,
#else
      0,
      0,
      0,
#endif  // HAS_DEV(GPS)
#if HAS_DEV(VBAT)
      (float)sensorData->vbatData[0],
#else
      0,
#endif  // HAS_DEV(VBAT)
      0,
      0,
#if HAS_DEV(GPS)
      (uint32_t)sensorData->gpsData[0].timeData.timestamp,
      sensorData->gpsData[0].generalData.satsTracked,
#else
      0,
      0,
#endif  // HAS_DEV(GPS)
      state,
      0  // TODO bluetooth clients
    };

    transmitPacket.packetType = TELEMETRY_ID_POSITION;
    transmitPacket.payload.positionData = data;

    sendInternal(transmitPacket);
  }

#if HAS_DEV(BAROMETER)
  if (timer.altInfoTimer.Expired(currentTime)) {
    timer.altInfoTimer.Reset();

    int num_baros = PRESSURE_MESSAGE_NUM_BAROMETERS;
    if (NUM_BAROMETER < PRESSURE_MESSAGE_NUM_BAROMETERS) {
      num_baros = NUM_BAROMETER;
    }

    AltInfoPacket_s data;
    for (int i = 0; i < num_baros; i++) {
      data.pressure[i] = (float)sensorData->barometerData[i].pressureAtm;
    }

    // Fill in the rest as 0s;
    for (int i = NUM_BAROMETER; i < PRESSURE_MESSAGE_NUM_BAROMETERS; i++) {
      data.pressure[i] = 0;
    }

    data.pressureRef = (float)filter_getPressureRef();
    data.groundElevation = (float)cli_getConfigs()->groundElevationM;
    data.groundTemp = (float)cli_getConfigs()->groundTemperatureC;

    transmitPacket.packetType = TELEMETRY_ID_ALT_INFO;
    transmitPacket.payload.altitudeInfo = data;

    sendInternal(transmitPacket);
  }
#endif  // HAS_DEV(BAROMETER)

#if HAS_DEV(PYRO_CONT)
  if (timer.pyroContTimer.Expired(currentTime)) {
    timer.pyroContTimer.Reset();

    uint8_t pyroCont = 0;
    for (int i = 0; i < NUM_PYRO_CONT; i++)
      pyroCont |= (uint8_t)((sensorData->pyroContData[i] & 0b1) << i);
    HardwareStatusPacket_s data = {pyroCont, triggerManager_status(),
                                   dataLog_getFlashUsage()};

    transmitPacket.packetType = TELEMETRY_ID_HARDWARE_STATUS;
    transmitPacket.payload.hardwareStatus = data;

    sendInternal(transmitPacket);
  }
#endif  // HAS_DEV(PYRO_CONT)

#if HAS_DEV(LINE_CUTTER)
  if (timer.lineCutterDataTimer.Expired(currentTime)) {
    timer.lineCutterDataTimer.Reset();
    for (int i = 0; i < NUM_LINE_CUTTER; i++) {
      transmitPacket.packetType = TELEMETRY_ID_LINECUTTER;
      transmitPacket.payload.lineCutter.data = *hm_getLineCutterData(i);

      sendInternal(transmitPacket);
    }
  }

  if (timer.lineCutterVarsTimer.Expired(currentTime)) {
    timer.lineCutterVarsTimer.Reset();
    for (int i = 0; i < NUM_LINE_CUTTER; i++) {
      transmitPacket.packetType = TELEMETRY_ID_LINECUTTER_VARS;
      transmitPacket.payload.lineCutterFlightVars.data =
          *hm_getLineCutterFlightVariables(i);

      sendInternal(transmitPacket);
    }
  }
#endif  // HAS_DEV(LINE_CUTTER)
}

void RadioManager::transmitString(uint8_t *data, size_t len) {
  static uint8_t lastTxId = 0;

  while (len) {
    size_t txLen = std::min(len, (size_t)RADIO_MAX_STRING);

    transmitPacket.timestampMs = hm_millis();
    transmitPacket.packetType = TELEMETRY_ID_STRING;

    // copy txLen many bytes into the string, and set the rest to null chars
    memset(transmitPacket.payload.cliString.string, 0, RADIO_MAX_STRING);
    memcpy(transmitPacket.payload.cliString.string, data, txLen);
    transmitPacket.payload.cliString.len = (uint8_t)txLen;
    transmitPacket.payload.cliString.id = lastTxId++;

#ifndef DESKTOP_SIM
    for (int i = 0; i < 3; i++) {
      // This is intended to be called twice to hopefully successfully send at
      // least once
      sendInternal(transmitPacket);
      sendInternal(transmitPacket);

      // The radio seems to not actually send the packet unless we actually call
      // RadioUpdate a bunch
      // TODO: This is a HACK
      uint32_t start;
      for (int i = 0; i < 9; i++) {
        hm_radioUpdate();
        start = hm_millis();
        while ((hm_millis() - start) < 5) {
        }
        hm_watchdogRefresh();
      }
    }
#else
    sendInternal(transmitPacket);
#endif

    len -= txLen;
    data += txLen;
  }
}

RadioManager radioManagers[NUM_RADIO];

void RadioManager::InitAll() {
  for (int i = 0; i < NUM_RADIO; i++) radioManagers[i].init(i);
}
void RadioManager::TickAll() {
  for (auto &m : radioManagers) m.tick();
}
