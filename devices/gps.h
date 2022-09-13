#ifndef DEVICES_GPS_H_
#define DEVICES_GPS_H_

#include <stdbool.h>

#include "board_config_common.h"
#include "data_structures.h"
#include "minmea.h"

#define GPS_RX_BUF_SIZE 4096
#define GPS_RX_BUF_HALF 2048

typedef enum { GPS_TYPE_STD = 0, GPS_TYPE_UBLOX } GpsType_e;

typedef struct {
  UART_HandleTypeDef *gps_uart;

  GpsType_e type;

  GpsData_s data;

  char rx_buff[GPS_RX_BUF_SIZE];
  char line[GPS_RX_BUF_HALF];

  int place;

  bool data_available;
  bool half;
} GpsCtrl_s;

void gps_init(GpsCtrl_s *gps, UART_HandleTypeDef *huart, GpsType_e type);

bool gps_newData(GpsCtrl_s *gps);

#endif  // DEVICES_GPS_H_
