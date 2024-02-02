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
#include "hm_timer.h"

#define RADIO_INTERVAL_MS 200
#define RADIO_SEND_MS 100

static DataRecieveState_s dataRx[NUM_RADIO];

static RadioDecodedPacket_s transmitPacket[NUM_RADIO];

static const char *call = "KM6GNL";

struct PacketTimerCollection {
  HmTickTimer orientationTimer{10};
  HmTickTimer positionTimer{10};
  HmTimer altInfoTimer{1213};
  HmTimer lineCutterDataTimer{1000};
  HmTimer lineCutterVarsTimer{1000};
  HmTickTimer pyroContTimer{1};
};

PacketTimerCollection timers[NUM_RADIO];

// https://stackoverflow.com/q/9695329
#define ROUND_2_INT(f) ((int)((f) >= 0.0 ? (f + 0.5) : (f - 0.5)))

void radioManager_init() {
  for (int i = 0; i < NUM_RADIO; i++) {
    cb_init(&(dataRx[i].rxBuffer), dataRx[i].rxArray, RX_BUFF_LEN,
            sizeof(RadioRecievedOTAPacket));
    hm_radioRegisterConsumer(i, &dataRx[i].rxBuffer);

    strncpy(transmitPacket[i].callsign, call, 8);

#ifdef SOFTWARE_VERSION
    transmitPacket[i].softwareVersion = SOFTWARE_VERSION;  // TODO set this;
#endif
    transmitPacket[i].board_serial_num = 0;  // TODO set this;

    hm_radioSetChannel(i, 2);
  }
}

//! Must be called periodically to output data over CLI or USB
void radioManager_tick() {
  static RadioRecievedOTAPacket packet;

  // Try to dequeue all the packets we've gotten
  for (int i = 0; i < NUM_RADIO; i++) {
    static size_t len = sizeof(packet);
    while (cb_count(&dataRx[i].rxBuffer)) {
      cb_peek(&dataRx[i].rxBuffer, (unknownPtr_t)&packet, &len);
      if (len) {
        // TODO - first decode the packet
        RadioDecodedRecievedPacket_s decoded;

        // Send to all our callbacks
        for (size_t j = 0; j < dataRx[i].numCallbacks; j++) {
          if (dataRx[i].callbacks[j]) dataRx[i].callbacks[j](&decoded);
        }

        cb_dequeue(&dataRx[i].rxBuffer, 1);
      } else {
        cb_flush(&dataRx[i].rxBuffer);
      }
    }
  }
}

void radioManager_sendInternal(int radioId) {
  RadioDecodedPacket_s &packet = transmitPacket[radioId];

  // hm_radioSend(radioId, (uint8_t *)&transmitPacket[radioId],
  //              sizeof(RadioPacket_s));
}

void radioManager_addMessageCallback(int radioId, RadioCallback_t callback) {
  dataRx[radioId].callbacks[dataRx[radioId].numCallbacks] = callback;
  dataRx[radioId].numCallbacks++;
}

void radioManager_transmitData(int radioId, SensorData_s *sensorData,
                               FilterData_s *filterData, uint8_t state) {
  uint32_t currentTime = hm_millis();

  if (currentTime % RADIO_INTERVAL_MS >= RADIO_SEND_MS) return;

  auto &timer = timers[radioId];
  transmitPacket[radioId].timestampMs = currentTime;

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
    transmitPacket[radioId].packetType = 2;
    transmitPacket[radioId].payload.orientation = data;

    radioManager_sendInternal(radioId);
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

    transmitPacket[radioId].packetType = TELEMETRY_ID_POSITION;
    transmitPacket[radioId].payload.positionData = data;

    radioManager_sendInternal(radioId);
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

    transmitPacket[radioId].packetType = TELEMETRY_ID_ALT_INFO;
    transmitPacket[radioId].payload.altitudeInfo = data;

    radioManager_sendInternal(radioId);
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

    transmitPacket[radioId].packetType = TELEMETRY_ID_HARDWARE_STATUS;
    transmitPacket[radioId].payload.hardwareStatus = data;

    radioManager_sendInternal(radioId);
  }
#endif  // HAS_DEV(PYRO_CONT)

#if HAS_DEV(LINE_CUTTER)
  if (timer.lineCutterDataTimer.Expired(currentTime)) {
    timer.lineCutterDataTimer.Reset();
    for (int i = 0; i < NUM_LINE_CUTTER; i++) {
      transmitPacket[radioId].packetType = TELEMETRY_ID_LINECUTTER;
      transmitPacket[radioId].payload.lineCutter.data =
          *hm_getLineCutterData(i);

      radioManager_sendInternal(radioId);
    }
  }

  if (timer.lineCutterVarsTimer.Expired(currentTime)) {
    timer.lineCutterVarsTimer.Reset();
    for (int i = 0; i < NUM_LINE_CUTTER; i++) {
      transmitPacket[radioId].packetType = TELEMETRY_ID_LINECUTTER_VARS;
      transmitPacket[radioId].payload.lineCutterFlightVars.data =
          *hm_getLineCutterFlightVariables(i);

      radioManager_sendInternal(radioId);
    }
  }
#endif  // HAS_DEV(LINE_CUTTER)
}

void radioManager_transmitString(int radioId, uint8_t *data, size_t len) {
  static uint8_t lastTxId = 0;

  while (len) {
    size_t txLen = std::min(len, (size_t)RADIO_MAX_STRING);

    transmitPacket[radioId].timestampMs = hm_millis();
    transmitPacket[radioId].packetType = TELEMETRY_ID_STRING;

    // copy txLen many bytes into the string, and set the rest to null chars
    memset(transmitPacket[radioId].payload.cliString.string, 0,
           RADIO_MAX_STRING);
    memcpy(transmitPacket[radioId].payload.cliString.string, data, txLen);
    transmitPacket[radioId].payload.cliString.len = (uint8_t)txLen;
    transmitPacket[radioId].payload.cliString.id = lastTxId++;

#ifndef DESKTOP_SIM
    for (int i = 0; i < 3; i++) {
      // This is intended to be called twice to hopefully successfully send at
      // least once
      radioManager_sendInternal(radioId);
      radioManager_sendInternal(radioId);

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
    radioManager_sendInternal(radioId);
#endif

    len -= txLen;
    data += txLen;
  }
}

void radioManager_transmitStringDefault(uint8_t *data, size_t len) {
  radioManager_transmitString(RADIO_CLI_ID, data, len);
}
