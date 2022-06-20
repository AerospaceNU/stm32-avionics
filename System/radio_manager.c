/*
 * radio_manager.c
 */

#include "radio_manager.h"

#include "board_config.h"
#include "cli.h"
#include "stdio.h"
#include "string.h"

static DataRecieveState_t dataRx;

static RadioPacket_t transmitPacket;

void RadioManager_init() {
  cbInit(&dataRx.rxBuffer, dataRx.rxArray, RX_BUFF_LEN,
         sizeof(RecievedPacket_t));
  HM_RadioRegisterConsumer(RADIO_433, &dataRx.rxBuffer);
  HM_RadioRegisterConsumer(RADIO_915, &dataRx.rxBuffer);

  const char *call = "KM6GNL";
  strncpy(transmitPacket.callsign, call, 8);

#ifdef SOFTWARE_VERSION
  transmitPacket.softwareVersion = SOFTWARE_VERSION;  // TODO set this;
#endif
  transmitPacket.board_serial_num = 0;  // TODO set this;

#ifdef TELEMETRY_RADIO
  HM_RadioSetChannel(TELEMETRY_RADIO, 2);
#endif
}

//! Must be called periodically to output data over CLI or USB
void RadioManager_tick() {
  static RecievedPacket_t packet;

  // Try to dequeue all the packet's we've gotten
  static size_t len = sizeof(packet);
  while (cbCount(&dataRx.rxBuffer)) {
    cbPeek(&dataRx.rxBuffer, &packet, &len);
    if (len) {
      // Send to all our callbacks
      for (int i = 0; i < dataRx.numCallbacks; i++) {
        if (dataRx.callbacks[i]) dataRx.callbacks[i](&packet);
      }

      cbDequeue(&dataRx.rxBuffer, 1);
    } else {
      cbFlush(&dataRx.rxBuffer);
    }
  }
}

void RadioManager_send_internal() {
#ifdef TELEMETRY_RADIO
  // if (true) {
  HM_RadioSend(TELEMETRY_RADIO, (uint8_t *)&transmitPacket, RADIO_PACKET_SIZE);
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
#endif
}

void RadioManager_addMessageCallback(RadioCallback_t callback) {
  dataRx.callbacks[dataRx.numCallbacks] = callback;
  dataRx.numCallbacks++;
}

// Packet rates, in hz
#define ORIENTATION_RATE 10
#define POSITION_RATE 10
#define PYRO_INFO_RATE 1

#ifdef TELEMETRY_RADIO
static DataTransmitState_t lastSent;
void RadioManager_transmitData(SensorData_t *sensorData,
                               FilterData_t *filterData, uint8_t state) {
  uint32_t currentTime = HM_Millis();
  transmitPacket.timestampMs = currentTime;

  /*
  if (currentTime - lastSent.propStuffLastSent >= 500) {
    PropulsionPacket_t data = {
        sensorData->loxTankDucer,  sensorData->kerTankDucer,
        sensorData->purgeDucer,    sensorData->loxInletDucer,
        sensorData->kerInletDucer, sensorData->loxVenturi,
        sensorData->kerVenturi,    sensorData->loadcell,
        sensorData->loxTank,       sensorData->injector,
        sensorData->engine};
    transmitPacket.packetType = 1;
    transmitPacket.payload = data;
    lastSent.propStuffLastSent = 0;
  }
  */

  if (currentTime - lastSent.orientationLastSent >= 1000 / ORIENTATION_RATE) {
    OrientationPacket_t data = {state,
                                (filterData->qw * 100.0),
                                (filterData->qx * 100.0),
                                filterData->qy * 100.0,
                                (filterData->qz * 100.0),
                                filterData->rocket_ang_vel_x,
                                filterData->rocket_ang_vel_y,
                                filterData->rocket_ang_vel_z,
                                filterData->world_acc_x,
                                filterData->world_acc_y,
                                filterData->world_acc_z,
                                sensorData->imu1_mag_x,
                                sensorData->imu1_mag_y,
                                sensorData->imu1_mag_z};
    transmitPacket.packetType = 2;
    transmitPacket.payload.orientation = data;
    lastSent.orientationLastSent = currentTime;

    RadioManager_send_internal(TELEMETRY_RADIO, (uint8_t *)&transmitPacket,
                               RADIO_PACKET_SIZE);
  }

  if (currentTime - lastSent.positionLastSent >= 1000 / POSITION_RATE) {
    PositionPacket_t data = {
        sensorData->baro1_temp,
        filterData->pos_z,
        filterData->rocket_vel_z,
        sensorData->gps_lat,
        sensorData->gps_long,
        sensorData->gps_alt,
        sensorData->battery_voltage,
        sensorData->gps_speed,
        sensorData->gps_course,
        sensorData->gps_timestamp,
        sensorData->gps_num_sats,
        state,
        0  // TODO bluetooth clients
    };

    transmitPacket.packetType = TELEMETRY_ID_POSITION;
    transmitPacket.payload.positionData = data;
    lastSent.positionLastSent = currentTime;

    RadioManager_send_internal(TELEMETRY_RADIO, (uint8_t *)&transmitPacket,
                               RADIO_PACKET_SIZE);
  }

  if (currentTime - lastSent.altInfoLastSent >= 1213) {
    AltInfoPacket_t data = {
        sensorData->baro1_pres, sensorData->baro2_pres, filterGetPressureRef(),
        cliGetConfigs()->groundElevationM, cliGetConfigs()->groundTemperatureC};

    transmitPacket.packetType = TELEMETRY_ID_ALT_INFO;
    transmitPacket.payload.altitudeInfo = data;
    lastSent.altInfoLastSent = currentTime;

    RadioManager_send_internal(TELEMETRY_RADIO, (uint8_t *)&transmitPacket,
                               RADIO_PACKET_SIZE);
  }

  if (currentTime - lastSent.pyroInfoLastSent >= 1000 / PYRO_INFO_RATE) {
    uint8_t pyroCont = 0;
    for (int i = 0; i < sizeof(sensorData->pyro_continuity); i++)
      pyroCont |= ((sensorData->pyro_continuity[i] & 0x01) << i);
    PyroInfoPacket_t data = {pyroCont, PyroManager_Status()};

    transmitPacket.packetType = TELEMETRY_ID_PYRO_INFO;
    transmitPacket.payload.pyroInfo = data;
    lastSent.pyroInfoLastSent = currentTime;

    RadioManager_send_internal(TELEMETRY_RADIO, (uint8_t *)&transmitPacket,
                               RADIO_PACKET_SIZE);
  }

#ifdef HAS_LINE_CUTTER
  if (currentTime - lastSent.lineCutterLastSent >= 1000) {
    for (int i = ADDR_CUTTER1; i <= ADDR_CUTTER2; i++) {
      transmitPacket.packetType = TELEMETRY_ID_LINECUTTER;
      transmitPacket.payload.lineCutter.data = *HM_GetLineCutterData(i);
      lastSent.lineCutterLastSent = currentTime;

      HM_RadioSend(TELEMETRY_RADIO, (uint8_t *)&transmitPacket,
                   RADIO_PACKET_SIZE);
    }
  }

  if (currentTime - lastSent.lineCutterVarsLastSent >= 10000) {
    for (int i = ADDR_CUTTER1; i <= ADDR_CUTTER2; i++) {
      transmitPacket.packetType = TELEMETRY_ID_LINECUTTER_VARS;
      transmitPacket.payload.lineCutterFlightVars.data =
          *HM_GetLineCutterFlightVariables(i);
      lastSent.lineCutterVarsLastSent = currentTime;

      HM_RadioSend(TELEMETRY_RADIO, (uint8_t *)&transmitPacket,
                   RADIO_PACKET_SIZE);
    }
  }
#endif
}

#endif

#define min(a, b) (a < b) ? a : b

void RadioManager_transmitString(Hardware_t radio, uint8_t *data, size_t len) {
  while (len) {
    uint8_t txLen = min(len, RADIO_MAX_STRING);

    transmitPacket.timestampMs = HM_Millis();
    transmitPacket.packetType = TELEMETRY_ID_STRING;

    // copy txLen many bytes into the string, and set the rest to null chars
    memset(transmitPacket.payload.cliString.string, 0, RADIO_MAX_STRING);
    memcpy(transmitPacket.payload.cliString.string, data, txLen);
    transmitPacket.payload.cliString.len = txLen;

    RadioManager_send_internal(radio, (uint8_t *)&transmitPacket,
                               RADIO_PACKET_SIZE);

    // The radio seems to not actually send the packet unless we actually call
    // RadioUpdate a bunch
    // TODO: This is a HACK
    for (int i = 0; i < 10; i++) {
      HM_RadioUpdate();
      uint32_t start = HM_Millis();
      while ((HM_Millis() - start) < 15) {
      }
      HM_IWDG_Refresh();
    }

    len -= txLen;
    data += txLen;
  }
}

void RadioManager_transmitStringDefault(uint8_t *data, size_t len) {
#ifdef TELEMETRY_RADIO
  RadioManager_transmitString(TELEMETRY_RADIO, data, len);
#endif
}
