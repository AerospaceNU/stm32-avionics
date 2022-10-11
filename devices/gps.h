#ifndef DEVICES_GPS_H_
#define DEVICES_GPS_H_

#include <stdbool.h>

#include "board_config_common.h"
#include "data_structures.h"
#include "minmea.h"

#define GPS_RX_BUF_SIZE 1024
#define NMEA_LENGTH 129  // +1 for null term

typedef enum { GPS_TYPE_STD = 0, GPS_TYPE_UBLOX } GpsType_e;

typedef struct {
  UART_HandleTypeDef *gps_uart;

  GpsType_e type;

  GpsData_s data;

  char rx_firstBuff[GPS_RX_BUF_SIZE];
  char rx_secondBuff[GPS_RX_BUF_SIZE];

  char line[NMEA_LENGTH];

  bool data_available;
  bool firstBuf;
} GpsCtrl_s;

void gps_init(GpsCtrl_s *gps, UART_HandleTypeDef *huart, GpsType_e type);

bool gps_newData(GpsCtrl_s *gps);

void gps_rxEventCallback(void *gps, size_t Size);

void gps_RxCpltCallback(void *gps);

#endif  // DEVICES_GPS_H_
