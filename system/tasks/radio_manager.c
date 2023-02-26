/*
 * radio_manager.c
 */

#include "radio_manager.h"

#include <stdio.h>
#include <string.h>

#include "board_config.h"
#include "board_config_common.h"
#include "cli.h"
#include "data_log.h"
#include "data_structures.h"
#include "hardware_manager.h"

#define RADIO_INTERVAL_MS 200
#define RADIO_SEND_MS 100

static DataRecieveState_s dataRx[NUM_RADIO];
static DataTransmitState_s lastSent[NUM_RADIO];

static RadioPacket_s transmitPacket[NUM_RADIO];

static const char *call = "KM6GNL";

void radioManager_init() {
  for (int i = 0; i < NUM_RADIO; i++) {
    cb_init(&(dataRx[i].rxBuffer), dataRx[i].rxArray, RX_BUFF_LEN,
            sizeof(RadioRecievedPacket_s));
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
  static RadioRecievedPacket_s packet;

  // Try to dequeue all the packets we've gotten
  for (int i = 0; i < NUM_RADIO; i++) {
    static size_t len = sizeof(packet);
    while (cb_count(&dataRx[i].rxBuffer)) {
      cb_peek(&dataRx[i].rxBuffer, &packet, &len);
      if (len) {
        // Send to all our callbacks
        for (int j = 0; j < dataRx[i].numCallbacks; j++) {
          if (dataRx[i].callbacks[j]) dataRx[i].callbacks[j](&packet);
        }

        cb_dequeue(&dataRx[i].rxBuffer, 1);
      } else {
        cb_flush(&dataRx[i].rxBuffer);
      }
    }
  }
}

void radioManager_sendInternal(int radioId) {
  hm_radioSend(radioId, (uint8_t *)&transmitPacket[radioId],
               sizeof(RadioPacket_s));
}

void radioManager_addMessageCallback(int radioId, RadioCallback_t callback) {
  dataRx[radioId].callbacks[dataRx[radioId].numCallbacks] = callback;
  dataRx[radioId].numCallbacks++;
}

// Packet rates, in hz
#define ORIENTATION_RATE 10
#define POSITION_RATE 10
#define HARDWARE_STATUS_RATE 1

void radioManager_transmitData(int radioId, SensorData_s *sensorData,
                               FilterData_s *filterData, uint8_t state) {
  uint32_t currentTime = hm_millis();
  if (currentTime % RADIO_INTERVAL_MS >= RADIO_SEND_MS) return;
  transmitPacket[radioId].timestampMs = currentTime;

  /*
  if (currentTime - lastSent[radioId].propStuffLastSent >= 500) {
    PropulsionPacket_s data = {
        sensorData->loxTankDucer,  sensorData->kerTankDucer,
        sensorData->purgeDucer,    sensorData->loxInletDucer,
        sensorData->kerInletDucer, sensorData->loxVenturi,
        sensorData->kerVenturi,    sensorData->loadcell,
        sensorData->loxTank,       sensorData->injector,
        sensorData->engine};
    transmitPacket[radioId].packetType = 1;
    transmitPacket[radioId].payload = data;
    lastSent[radioId].propStuffLastSent = 0;
    radioManager_sendInternal(radioId);
  }
  */

  if (currentTime - lastSent[radioId].orientationLastSent >=
      1000 / ORIENTATION_RATE) {
    OrientationPacket_s data = {
      state,
      filterData->qw * 100.0,
      filterData->qx * 100.0,
      filterData->qy * 100.0,
      filterData->qz * 100.0,
      filterData->rocket_ang_vel_x,
      filterData->rocket_ang_vel_y,
      filterData->rocket_ang_vel_z,
      filterData->world_acc_x,
      filterData->world_acc_y,
      filterData->world_acc_z,
#if HAS_DEV(IMU)
      sensorData->imuData[0].magRealG.x,
      sensorData->imuData[0].magRealG.y,
      sensorData->imuData[0].magRealG.z,
#else
      0,
      0,
      0,
#endif  // HAS_DEV(IMU)
    };
    transmitPacket[radioId].packetType = 2;
    transmitPacket[radioId].payload.orientation = data;
    lastSent[radioId].orientationLastSent = currentTime;

    radioManager_sendInternal(radioId);
  }

  if (currentTime - lastSent[radioId].positionLastSent >=
      1000 / POSITION_RATE) {
    PositionPacket_s data = {
#if HAS_DEV(BAROMETER)
      sensorData->barometerData[0].temperatureC,
#else
      0,
#endif  // HAS_DEV(BAROMETER)
      filterData->pos_z_agl,
      filterData->world_vel_z,
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
      sensorData->vbatData[0],
#else
      0,
#endif  // HAS_DEV(VBAT)
      0,
      0,
#if HAS_DEV(GPS)
      sensorData->gpsData[0].timeData.timestamp,
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
    lastSent[radioId].positionLastSent = currentTime;

    radioManager_sendInternal(radioId);
  }

#if HAS_DEV(BAROMETER)
  if (currentTime - lastSent[radioId].altInfoLastSent >= 1213) {
    AltInfoPacket_s data;
    for (int i = 0; i < NUM_BAROMETER; i++) {
      data.pressure[i] = sensorData->barometerData[i].pressureAtm;
    }
    data.pressureRef = filter_getPressureRef();
    data.groundElevation = cli_getConfigs()->groundElevationM;
    data.groundTemp = cli_getConfigs()->groundTemperatureC;

    transmitPacket[radioId].packetType = TELEMETRY_ID_ALT_INFO;
    transmitPacket[radioId].payload.altitudeInfo = data;
    lastSent[radioId].altInfoLastSent = currentTime;

    radioManager_sendInternal(radioId);
  }
#endif  // HAS_DEV(BAROMETER)

#if HAS_DEV(PYRO_CONT)
  if (currentTime - lastSent[radioId].hardwareStatusLastSent >=
      1000 / HARDWARE_STATUS_RATE) {
    uint8_t pyroCont = 0;
    for (int i = 0; i < NUM_PYRO_CONT; i++)
      pyroCont |= ((sensorData->pyroContData[i] & 0x01) << i);
    HardwareStatusPacket_s data = {pyroCont, triggerManager_status(),
                                   dataLog_getFlashUsage()};

    transmitPacket[radioId].packetType = TELEMETRY_ID_HARDWARE_STATUS;
    transmitPacket[radioId].payload.hardwareStatus = data;
    lastSent[radioId].hardwareStatusLastSent = currentTime;

    radioManager_sendInternal(radioId);
  }
#endif  // HAS_DEV(PYRO_CONT)

#if HAS_DEV(LINE_CUTTER)
  if (currentTime - lastSent[radioId].lineCutterLastSent >= 1000) {
    for (int i = 0; i < NUM_LINE_CUTTER; i++) {
      transmitPacket[radioId].packetType = TELEMETRY_ID_LINECUTTER;
      transmitPacket[radioId].payload.lineCutter.data =
          *hm_getLineCutterData(i);
      lastSent[radioId].lineCutterLastSent = currentTime;

      radioManager_sendInternal(radioId);
    }
  }

  if (currentTime - lastSent[radioId].lineCutterVarsLastSent >= 10000) {
    for (int i = 0; i < NUM_LINE_CUTTER; i++) {
      transmitPacket[radioId].packetType = TELEMETRY_ID_LINECUTTER_VARS;
      transmitPacket[radioId].payload.lineCutterFlightVars.data =
          *hm_getLineCutterFlightVariables(i);
      lastSent[radioId].lineCutterVarsLastSent = currentTime;

      radioManager_sendInternal(radioId);
    }
  }
#endif  // HAS_DEV(LINE_CUTTER)
}

#define min(a, b) (a < b) ? a : b

void radioManager_transmitString(int radioId, uint8_t *data, size_t len) {
  static uint8_t lastTxId = 0;

  while (len) {
    uint8_t txLen = min(len, RADIO_MAX_STRING);

    transmitPacket[radioId].timestampMs = hm_millis();
    transmitPacket[radioId].packetType = TELEMETRY_ID_STRING;

    // copy txLen many bytes into the string, and set the rest to null chars
    memset(transmitPacket[radioId].payload.cliString.string, 0,
           RADIO_MAX_STRING);
    memcpy(transmitPacket[radioId].payload.cliString.string, data, txLen);
    transmitPacket[radioId].payload.cliString.len = txLen;
    transmitPacket[radioId].payload.cliString.id = lastTxId++;

#if DO_RADIO_HACK == 1
    for (int i = 0; i < 3; i++) {
#else
    {
#endif  // DO_RADIO_HACK

#if DO_RADIO_HACK == 1
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

#else
      radioManager_sendInternal(radioId);
#endif  // DO_RADIO_HACK
    }

    len -= txLen;
    data += txLen;
  }
}

void radioManager_transmitStringDefault(uint8_t *data, size_t len) {
  radioManager_transmitString(RADIO_CLI_ID, data, len);
}
