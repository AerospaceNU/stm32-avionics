/*
 * radio_manager.h
 */

#ifndef SYSTEM_RADIO_MANAGER_H_
#define SYSTEM_RADIO_MANAGER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "board_config.h"
#include "data_structures.h"
#include "filters.h"
#include "hardware_manager.h"

#define PACKED_STRUCT typedef struct __attribute__((__packed__))

#define RADIO_MAX_STRING \
  48  // The maximum length one string sent over radio can be. Longer must be
      // segmented
#define RX_BUFF_LEN 15

// Prop data, from the link budget Google sheet
#define TELEMETRY_ID_PROPSTUFF 1
PACKED_STRUCT {
  // float loxTankDucer, kerTankDucer, purgeDucer, loxInletDucer, kerInletDucer,
  // loxVenturi, kerVenturi, loadcell, loxTank, injector, engine;
  //  etc
}
PropulsionPacket_t;

// Orientation, angular rates
#define TELEMETRY_ID_ORIENTATION 2
PACKED_STRUCT {
  uint8_t state;
  int8_t qw, qx, qy, qz;
  float wx, wy, wz, ax, ay, az, bx, by, bz;
}
OrientationPacket_t;

// Location data?? from the link budget Google sheet
#define TELEMETRY_ID_POSITION 3
PACKED_STRUCT {
  float temp, pos_z, vel_z, lat, lon, gps_alt, batt_volts, speedKnots,
      courseDeg;
  uint32_t gpsTime;
  uint8_t sats, state, btClients;
  // etc
}
PositionPacket_t;

// Line cutter (x2)
#define TELEMETRY_ID_LINECUTTER 4
PACKED_STRUCT {
  uint8_t lineCutterNumber, state;
  uint16_t battSense, cutSense1, cutSense2, currentSense, photoresistor;
  uint32_t timestamp, pressure;
  float altitude, avgAltitude, deltaAltitude, avgDeltaAltitude, temperature,
      accelX, accelY, accelZ;
}
LineCutterPacket_t;

// Uplinked string (not necessarily null-terminated)
#define TELEMETRY_ID_STRING 5
#define RADIO_MAX_STRING 48
PACKED_STRUCT {
  uint8_t len;
  uint8_t message[RADIO_MAX_STRING];
}
CliStringPacket_t;

#define TELEMETRY_ID_ALT_INFO 6
PACKED_STRUCT {
  float pressure1, pressure2, pressureRef, groundElevation, groundTemp;
}
AltInfoPacket_t;

#define TELEMETRY_ID_PYRO_INFO 7
PACKED_STRUCT {
  uint8_t pyroContinuity;
  uint8_t pyroFireStatus;
}
PyroInfoPacket_t;

typedef union {
  PropulsionPacket_t propStuff;
  OrientationPacket_t orientation;
  PositionPacket_t positionData;
  LineCutterPacket_t lineCutter;
  CliStringPacket_t cliString;
  AltInfoPacket_t altitudeInfo;
  PyroInfoPacket_t pyroInfo;
} PayloadPacket_u;

PACKED_STRUCT {
  uint8_t packetType;
  uint8_t softwareVersion;
  uint8_t board_serial_num;
  uint32_t timestampMs;
  char callsign[8];
  PayloadPacket_u payload;
}
RadioPacket_t;
#define RADIO_PACKET_SIZE sizeof(RadioPacket_t)

// Struct to keep track of when messages were last sent
typedef struct {
  uint32_t propStuffLastSent;
  uint32_t orientationLastSent;
  uint32_t positionLastSent;
  uint32_t lineCutterLastSent;
  uint32_t altInfoLastSent;
  uint32_t pyroInfoLastSent;
} DataTransmitState_t;

#define RADIO_MAX_CALLBACKS 10
typedef void (*RadioCallback_t)(RecievedPacket_t*);

typedef struct {
  // The radio will enqueue packets here automatically
  CircularBuffer_t rxBuffer;
  uint8_t rxArray[RX_BUFF_LEN * sizeof(RecievedPacket_t)];

  RadioCallback_t callbacks[RADIO_MAX_CALLBACKS];
  size_t numCallbacks;
} DataRecieveState_t;

void RadioManager_init();
void RadioManager_tick();

void RadioManager_transmitData(SensorData_t* sensorData,
                               FilterData_t* filterData, uint8_t state);
void RadioManager_transmitString(Hardware_t radio, uint8_t* data, size_t len);

void RadioManager_addMessageCallback(RadioCallback_t callback);

#ifdef __cplusplus
}
#endif

#endif  // SYSTEM_RADIO_MANAGER_H_
