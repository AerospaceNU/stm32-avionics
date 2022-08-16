#ifndef DEVICES_GPS_H_
#define DEVICES_GPS_H_

#include "board_config_common.h"
#include "data_structures.h"
#include "minmea.h"
#include "stdbool.h"
#include "string.h"

#define GPS_RX_BUF_SIZE 4096
#define GPS_RX_BUF_HALF 2048

typedef enum { GPS_TYPE_STD = 0, GPS_TYPE_UBLOX } GpsType_te;

typedef struct GPSCtrl_s {
  UART_HandleTypeDef *gps_uart;

  GpsType_te type;

  GpsData_t data;

  char rx_buff[GPS_RX_BUF_SIZE];
  char line[GPS_RX_BUF_HALF];

  int place;

  bool data_available;
  bool half;
} GpsCtrl_t;

void gps_init(GpsCtrl_t *gps, UART_HandleTypeDef *huart, GpsType_te type);

void gps_process_data(GpsCtrl_t *gps);

bool gps_new_data(GpsCtrl_t *gps);

#endif  // DEVICES_GPS_H_
