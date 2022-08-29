/*
 * radio_manager.c
 */

#include "radio_manager.h"

#include <stdio.h>
#include <string.h>

#include "board_config_common.h"
#include "cli.h"
#include "data_structures.h"
#include "hardware_manager.h"

static DataRecieveState_t dataRx[NUM_RADIO];
static DataTransmitState_t lastSent[NUM_RADIO];

static RadioPacket_t transmitPacket[NUM_RADIO];

static const char *call = "KM6GNL";

void RadioManager_init() {
  for (int i = 0; i < NUM_RADIO; i++) {
    cbInit(&(dataRx[i].rxBuffer), dataRx[i].rxArray, RX_BUFF_LEN,
           sizeof(RecievedPacket_t));
    HM_RadioRegisterConsumer(i, &dataRx[i].rxBuffer);

    strncpy(transmitPacket[i].callsign, call, 8);

#ifdef SOFTWARE_VERSION
    transmitPacket[i].softwareVersion = SOFTWARE_VERSION;  // TODO set this;
#endif
    transmitPacket[i].board_serial_num = 0;  // TODO set this;

    HM_RadioSetChannel(i, 2);
  }
}

//! Must be called periodically to output data over CLI or USB
void RadioManager_tick() {
  static RecievedPacket_t packet;

  // Try to dequeue all the packets we've gotten
  for (int i = 0; i < NUM_RADIO; i++) {
    static size_t len = sizeof(packet);
    while (cbCount(&dataRx[i].rxBuffer)) {
      cbPeek(&dataRx[i].rxBuffer, &packet, &len);
      if (len) {
        // Send to all our callbacks
        for (int j = 0; j < dataRx[i].numCallbacks; j++) {
          if (dataRx[i].callbacks[j]) dataRx[i].callbacks[j](&packet);
        }

        cbDequeue(&dataRx[i].rxBuffer, 1);
      } else {
        cbFlush(&dataRx[i].rxBuffer);
      }
    }
  }
}

void RadioManager_send_internal(int radioId) {
  // if (true) {
  HM_RadioSend(radioId, (uint8_t *)&transmitPacket[radioId],
               sizeof(RadioPacket_t));
  /*
   } else {
     static RecievedPacket_t packet;
     packet.crc = true;
     packet.lqi = 1;
     packet.rssi = 1;
     packet.radioId = TELEMETRY_RADIO;
     memcpy(packet.data, &transmitPacket, RADIO_PACKET_SIZE);
     HM_UsbTransmit((uint8_t *)&packet, sizeof(packet));
   }
   */
}

void RadioManager_addMessageCallback(int radioId, RadioCallback_t callback) {
  dataRx[radioId].callbacks[dataRx[radioId].numCallbacks] = callback;
  dataRx[radioId].numCallbacks++;
}

// Packet rates, in hz
#define ORIENTATION_RATE 10
#define POSITION_RATE 10
#define PYRO_INFO_RATE 1

void RadioManager_transmitData(int radioId, SensorData_t *sensorData,
                               FilterData_t *filterData, uint8_t state) {
  uint32_t currentTime = HM_Millis();
  transmitPacket[radioId].timestampMs = currentTime;

  /*
  if (currentTime - lastSent[radioId].propStuffLastSent >= 500) {
    PropulsionPacket_t data = {
        sensorData->loxTankDucer,  sensorData->kerTankDucer,
        sensorData->purgeDucer,    sensorData->loxInletDucer,
        sensorData->kerInletDucer, sensorData->loxVenturi,
        sensorData->kerVenturi,    sensorData->loadcell,
        sensorData->loxTank,       sensorData->injector,
        sensorData->engine};
    transmitPacket[radioId].packetType = 1;
    transmitPacket[radioId].payload = data;
    lastSent[radioId].propStuffLastSent = 0;
    RadioManager_send_internal(radioId);
  }
  */

  if (currentTime - lastSent[radioId].orientationLastSent >=
      1000 / ORIENTATION_RATE) {
    OrientationPacket_t data = {
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

    RadioManager_send_internal(radioId);
  }

  if (currentTime - lastSent[radioId].positionLastSent >=
      1000 / POSITION_RATE) {
    PositionPacket_t data = {
#if HAS_DEV(BAROMETER)
      sensorData->barometerData[0].temperatureC,
#else
      0,
#endif  // HAS_DEV(BAROMETER)
      filterData->pos_z,
      filterData->rocket_vel_z,
#if HAS_DEV(GPS)
      sensorData->gpsData[0].latitude,
      sensorData->gpsData[0].longitude,
      sensorData->gpsData[0].altitude,
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
#if HAS_DEV(GPS)
      sensorData->gpsData[0].speedKnots,
      sensorData->gpsData[0].courseDeg,
      sensorData->gpsData[0].timestamp,
      sensorData->gpsData[0].num_sats,
#else
      0,
      0,
      0,
      0,
#endif  // HAS_DEV(GPS)
      state,
      0  // TODO bluetooth clients
    };

    transmitPacket[radioId].packetType = TELEMETRY_ID_POSITION;
    transmitPacket[radioId].payload.positionData = data;
    lastSent[radioId].positionLastSent = currentTime;

    RadioManager_send_internal(radioId);
  }

#if HAS_DEV(BAROMETER)
  if (currentTime - lastSent[radioId].altInfoLastSent >= 1213) {
    AltInfoPacket_t data;
    for (int i = 0; i < NUM_BAROMETER; i++) {
      data.pressure[i] = sensorData->barometerData[i].pressureAtm;
    }
    data.pressureRef = filterGetPressureRef();
    data.groundElevation = cliGetConfigs()->groundElevationM;
    data.groundTemp = cliGetConfigs()->groundTemperatureC;

    transmitPacket[radioId].packetType = TELEMETRY_ID_ALT_INFO;
    transmitPacket[radioId].payload.altitudeInfo = data;
    lastSent[radioId].altInfoLastSent = currentTime;

    RadioManager_send_internal(radioId);
  }
#endif  // HAS_DEV(BAROMETER)

#if HAS_DEV(PYRO_CONT)
  if (currentTime - lastSent[radioId].pyroInfoLastSent >=
      1000 / PYRO_INFO_RATE) {
    uint8_t pyroCont = 0;
    for (int i = 0; i < NUM_PYRO_CONT; i++)
      pyroCont |= ((sensorData->pyroContData[i] & 0x01) << i);
    PyroInfoPacket_t data = {pyroCont, PyroManager_Status()};

    transmitPacket[radioId].packetType = TELEMETRY_ID_PYRO_INFO;
    transmitPacket[radioId].payload.pyroInfo = data;
    lastSent[radioId].pyroInfoLastSent = currentTime;

    RadioManager_send_internal(radioId);
  }
#endif  // HAS_DEV(PYRO_CONT)

#if HAS_DEV(LINE_CUTTER)
  if (currentTime - lastSent[radioId].lineCutterLastSent >= 1000) {
    for (int i = 0; i <= NUM_LINE_CUTTER; i++) {
      transmitPacket[radioId].packetType = TELEMETRY_ID_LINECUTTER;
      transmitPacket[radioId].payload.lineCutter.data =
          *HM_GetLineCutterData(i);
      lastSent[radioId].lineCutterLastSent = currentTime;

      RadioManager_send_internal(radioId);
    }
  }

  if (currentTime - lastSent[radioId].lineCutterVarsLastSent >= 10000) {
    for (int i = 0; i <= NUM_LINE_CUTTER; i++) {
      transmitPacket[radioId].packetType = TELEMETRY_ID_LINECUTTER_VARS;
      transmitPacket[radioId].payload.lineCutterFlightVars.data =
          *HM_GetLineCutterFlightVariables(i);
      lastSent[radioId].lineCutterVarsLastSent = currentTime;

      RadioManager_send_internal(radioId);
    }
  }
#endif  // HAS_DEV(LINE_CUTTER)
}

#define min(a, b) (a < b) ? a : b

void RadioManager_transmitString(int radioId, uint8_t *data, size_t len) {
  while (len) {
    uint8_t txLen = min(len, RADIO_MAX_STRING);

    transmitPacket[radioId].timestampMs = HM_Millis();
    transmitPacket[radioId].packetType = TELEMETRY_ID_STRING;

    // copy txLen many bytes into the string, and set the rest to null chars
    memset(transmitPacket[radioId].payload.cliString.string, 0,
           RADIO_MAX_STRING);
    memcpy(transmitPacket[radioId].payload.cliString.string, data, txLen);
    transmitPacket[radioId].payload.cliString.len = txLen;

    RadioManager_send_internal(radioId);

    // The radio seems to not actually send the packet unless we actually call
    // RadioUpdate a bunch
    // TODO: This is a HACK
    for (int i = 0; i < 10; i++) {
      HM_RadioUpdate();
      uint32_t start = HM_Millis();
      while ((HM_Millis() - start) < 15) {
      }
      HM_WatchdogRefresh();
    }

    len -= txLen;
    data += txLen;
  }
}

void RadioManager_transmitStringDefault(uint8_t *data, size_t len) {
  RadioManager_transmitString(RADIO_CLI_ID, data, len);
}
