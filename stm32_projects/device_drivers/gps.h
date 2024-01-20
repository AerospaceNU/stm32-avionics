#ifndef STM32_PROJECTS_DEVICE_DRIVERS_GPS_H_
#define STM32_PROJECTS_DEVICE_DRIVERS_GPS_H_

#include <stdbool.h>

#include "board_config_common.h"
#include "data_structures.h"
#include "minmea.h"

#if (HAS_DEV(GPS_STD) || HAS_DEV(GPS_UBLOX))
#define GPS_RX_BUF_SIZE 1024
#define NMEA_LENGTH 129  // +1 for null term

typedef enum { GPS_TYPE_STD = 0, GPS_TYPE_UBLOX } GpsType_e;

typedef struct {
  UART_HandleTypeDef *gps_uart;

  GpsType_e type;

  GpsData_s data;

  char rx_firstBuff[GPS_RX_BUF_SIZE];
  char rx_secondBuff[GPS_RX_BUF_SIZE];
  int lastBufferedSize;

  char line[NMEA_LENGTH];

  bool data_available;
  bool firstBuf;
} GpsCtrl_s;

void gps_init(GpsCtrl_s *gps, UART_HandleTypeDef *huart, GpsType_e type);

bool gps_newData(GpsCtrl_s *gps);

void gps_rxEventCallback(void *gps, size_t Size);

void gps_RxCpltCallback(void *gps);

void gps_setRate(GpsCtrl_s *gps, uint16_t rate);

void gps_setMessagesUsed(GpsCtrl_s *gps);

void gps_enable4g(GpsCtrl_s *gps);

void gps_addUbxChecksum(uint8_t *data, int len);

#endif

#endif  // STM32_PROJECTS_DEVICE_DRIVERS_GPS_H_
