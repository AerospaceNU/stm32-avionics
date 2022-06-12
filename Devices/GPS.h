#ifndef DEVICES_GPS_H_
#define DEVICES_GPS_H_

#include "board_config.h"
#include HAL_HEADER

#include "minmea.h"
#include "stdbool.h"
#include "string.h"

#define GPS_RX_BUF_SIZE 4096
#define GPS_RX_BUF_HALF 2048

typedef enum { GPS_TYPE_UBLOX = 0 } GPSType_e;

typedef struct GPSCtrl_s {
  UART_HandleTypeDef *gps_uart;
  DMA_HandleTypeDef *gpa_dma_rx;

  GPSType_e type;

  float latitude;
  float longitude;
  float speed;   // knots
  float course;  // True course, degrees
  float altitude;
  float latitude_deviation;
  float longitude_deviation;
  float altitude_deviation;
  float speed_kph;
  float speed_knots;
  uint64_t timestamp;
  int seconds;
  int microseconds;
  int minutes;
  int hours;
  int day;
  int month;
  int year;
  int num_sats;
  char status;

  char rx_buff[GPS_RX_BUF_SIZE];
  char line[GPS_RX_BUF_HALF];

  int place;

  bool data_available;
  bool half;
} GPSCtrl_t;

void parseString(GPSCtrl_t *gps, char line[]);

void gps_process_data(GPSCtrl_t *gps);

void gps_init(GPSCtrl_t *gps);

bool gps_new_data(GPSCtrl_t *gps);

#endif  // DEVICES_GPS_H_
