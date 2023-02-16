/*
 * radio_packet_types.h
 */

#ifndef SYSTEM_TASKS_RADIO_PACKET_TYPES_H_
#define SYSTEM_TASKS_RADIO_PACKET_TYPES_H_

#include <stdint.h>

#include "board_config_common.h"

#define PACKED_STRUCT typedef struct __attribute__((__packed__))

#define RX_BUFF_LEN 15

// Prop data, from the link budget Google sheet
#define TELEMETRY_ID_PROPSTUFF 1
PACKED_STRUCT {
  // float loxTankDucer, kerTankDucer, purgeDucer, loxInletDucer, kerInletDucer,
  // loxVenturi, kerVenturi, loadcell, loxTank, injector, engine;
  //  etc
}
PropulsionPacket_s;

// Orientation, angular rates
#define TELEMETRY_ID_ORIENTATION 2
PACKED_STRUCT {
  uint8_t state;
  int8_t qw, qx, qy, qz;
  float wx, wy, wz, ax, ay, az, bx, by, bz;
}
OrientationPacket_s;

// Location data?? from the link budget Google sheet
#define TELEMETRY_ID_POSITION 3
PACKED_STRUCT {
  float temp, pos_z, vel_z, lat, lon, gps_alt, batt_volts, speedKnots,
      courseDeg;
  uint32_t gpsTime;
  uint8_t sats, state, btClients;
  // etc
}
PositionPacket_s;

#define TELEMETRY_ID_LINECUTTER 4
PACKED_STRUCT { LineCutterData_s data; }
LineCutterPacket_s;

// Uplinked string (not necessarily null-terminated)
#define TELEMETRY_ID_STRING 5
#define RADIO_MAX_STRING 48
PACKED_STRUCT {
  uint8_t len;
  uint8_t id;
  uint8_t string[RADIO_MAX_STRING];
}
CliStringPacket_s;

#if HAS_DEV(BAROMETER)
#define TELEMETRY_ID_ALT_INFO 6
PACKED_STRUCT {
  float pressure[NUM_BAROMETER];
  float pressureRef, groundElevation, groundTemp;
}
AltInfoPacket_s;
#endif  // HAS_DEV(BAROMETER)

#define TELEMETRY_ID_HARDWARE_STATUS 7
PACKED_STRUCT {
  uint8_t pyroContinuity;
  uint16_t triggerFireStatus;
  uint8_t flashUsage;
}
HardwareStatusPacket_s;

#define TELEMETRY_ID_LINECUTTER_VARS 8
PACKED_STRUCT { LineCutterFlightVars_s data; }
LineCutterVarsPacket_s;

typedef union {
  PropulsionPacket_s propStuff;
  OrientationPacket_s orientation;
  PositionPacket_s positionData;
  LineCutterPacket_s lineCutter;
  LineCutterVarsPacket_s lineCutterFlightVars;
  CliStringPacket_s cliString;
#if HAS_DEV(BAROMETER)
  AltInfoPacket_s altitudeInfo;
#endif  // HAS_DEV(BAROMETER)
  HardwareStatusPacket_s hardwareStatus;
} PayloadPacket_u;

PACKED_STRUCT {
  uint8_t packetType;
  uint8_t softwareVersion;
  uint8_t board_serial_num;
  uint32_t timestampMs;
  char callsign[8];
  PayloadPacket_u payload;
}
RadioPacket_s;

#endif  // SYSTEM_TASKS_RADIO_PACKET_TYPES_H_
