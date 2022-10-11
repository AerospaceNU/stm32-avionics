#include "gps.h"

#if HAS_DEV(GPS_STD) || HAS_DEV(GPS_UBLOX)

#include <string.h>

#include "hal_callbacks.h"
#include "minmea.h"

#define GPS_MIN_VALID_YEAR 2000
#define GPS_MAX_VALID_YEAR 2100

#define GPS_UART_TIMEOUT_MS 50

char *gps_getActiveBuff(GpsCtrl_s *gps, bool swapBuff) {
  bool desiredBuff = swapBuff ? gps->firstBuf : !gps->firstBuf;
  return desiredBuff ? gps->rx_firstBuff : gps->rx_secondBuff;
}

static void parseString(GpsCtrl_s *gps, char line[]) {
  switch (minmea_sentence_id(line, false)) {
    case MINMEA_SENTENCE_RMC: {
      struct minmea_sentence_rmc frame0;
      if (minmea_parse_rmc(&frame0, line)) {
        gps->data.hours = frame0.time.hours;
        gps->data.minutes = frame0.time.minutes;
        gps->data.seconds = frame0.time.seconds;
        gps->data.speedKnots = minmea_tofloat(&frame0.speed);
        gps->data.courseDeg = minmea_tofloat(&frame0.course);
        if (!isnan(minmea_tofloat(&frame0.latitude)) &&
            !isnan(minmea_tofloat(&frame0.longitude))) {
          gps->data.latitude = minmea_tofloat(&frame0.latitude);
          gps->data.longitude = minmea_tofloat(&frame0.longitude);
        }
      }
      break;
    }
    case MINMEA_SENTENCE_GGA: {
      struct minmea_sentence_gga frame1;
      if (minmea_parse_gga(&frame1, line)) {
        gps->data.hours = frame1.time.hours;
        gps->data.minutes = frame1.time.minutes;
        gps->data.seconds = frame1.time.seconds;
        if (!isnan(minmea_tofloat(&frame1.latitude)) &&
            !isnan(minmea_tofloat(&frame1.longitude)) &&
            !isnan(minmea_tofloat(&frame1.altitude))) {
          gps->data.latitude = minmea_tofloat(&frame1.latitude);
          gps->data.longitude = minmea_tofloat(&frame1.longitude);
          gps->data.altitude = minmea_tofloat(&frame1.altitude);
        }
      }

      break;
    }
    case MINMEA_SENTENCE_GLL: {
      struct minmea_sentence_gll frame2;
      if (minmea_parse_gll(&frame2, line)) {
        gps->data.hours = frame2.time.hours;
        gps->data.minutes = frame2.time.minutes;
        gps->data.seconds = frame2.time.seconds;
        gps->data.status = frame2.status;
        if (!isnan(minmea_tofloat(&frame2.latitude)) &&
            !isnan(minmea_tofloat(&frame2.longitude))) {
          gps->data.latitude = minmea_tofloat(&frame2.latitude);
          gps->data.longitude = minmea_tofloat(&frame2.longitude);
        }
      }
      break;
    }
    case MINMEA_SENTENCE_GSA: {
      break;
    }
    case MINMEA_SENTENCE_GST: {
      struct minmea_sentence_gst frame3;
      if (minmea_parse_gst(&frame3, line)) {
        gps->data.hours = frame3.time.hours;
        gps->data.minutes = frame3.time.minutes;
        gps->data.seconds = frame3.time.seconds;
        gps->data.latitudeDeviation =
            minmea_tofloat(&frame3.latitude_error_deviation);
        gps->data.longitudeDeviation =
            minmea_tofloat(&frame3.longitude_error_deviation);
        gps->data.altitudeDeviation =
            minmea_tofloat(&frame3.altitude_error_deviation);
      }
      break;
    }
    case MINMEA_SENTENCE_GSV: {
      struct minmea_sentence_gsv frame4;
      if (minmea_parse_gsv(&frame4, line)) {
        gps->data.num_sats = frame4.total_sats;
      }
      break;
    }
    case MINMEA_SENTENCE_VTG: {
      struct minmea_sentence_vtg frame5;
      if (minmea_parse_vtg(&frame5, line)) {
        gps->data.speedKph = minmea_tofloat(&frame5.speed_kph);
        gps->data.speedKnots2 = minmea_tofloat(&frame5.speed_knots);
      }
      break;
    }
    case MINMEA_SENTENCE_ZDA: {
      struct minmea_sentence_zda frame6;
      if (minmea_parse_zda(&frame6, line)) {
        gps->data.hours = frame6.time.hours;
        gps->data.minutes = frame6.time.minutes;
        gps->data.seconds = frame6.time.seconds;
        gps->data.microseconds = frame6.time.microseconds;
        gps->data.day = frame6.date.day;
        gps->data.month = frame6.date.month;
        gps->data.year = frame6.date.year;

        struct timespec ts;
        memset(&ts, 0, sizeof(ts));

        const struct minmea_date date = {gps->data.day, gps->data.month,
                                         gps->data.year};
        const struct minmea_time time = {gps->data.hours, gps->data.minutes,
                                         gps->data.seconds,
                                         gps->data.microseconds};

        if (!minmea_gettime(&ts, &date, &time) &&
            gps->data.year > GPS_MIN_VALID_YEAR &&
            gps->data.year < GPS_MAX_VALID_YEAR) {
          gps->data.timestamp = ts.tv_sec;
        }
      }
      break;
    }
    default: {
    }
  }
}

static void gps_processData(GpsCtrl_s *gps) {
  // Get the buffer to use
  // The false flips which buffer we get, as we want to parse
  // the inactive buffer, not the active buffer
  char *buff = gps_getActiveBuff(gps, false);

  // Iterate over the entire buffer
  for (int i = 0; i < GPS_RX_BUF_SIZE;) {
    // Make sure we are at the start of a sentence
    if (buff[i] == '$') {
      // Make a copy and null terminate the line
      for (int j = 0; j < NMEA_LENGTH - 2 && i < GPS_RX_BUF_SIZE; j++, i++) {
        gps->line[j] = buff[i];
        // If we made it to the end of a sentence
        if (buff[i] == '\n') {
          gps->line[j + 1] = '\0';
          parseString(gps, gps->line);
          break;
        }
      }
    } else {
      i++;
    }
  }

  // Reset buffer variables
  memset(buff, 0, GPS_RX_BUF_SIZE);
  gps->data_available = false;
}

void gps_rxEventCallback(void *gps, size_t Size) {
  // GPS data received
  GpsCtrl_s *pgps = (GpsCtrl_s *)gps;
  pgps->data_available = true;

  // Swap the buffer
  pgps->firstBuf = !pgps->firstBuf;

  // Reconfigure the UART
  HAL_UARTEx_ReceiveToIdle_DMA(pgps->gps_uart,
                               (uint8_t *)gps_getActiveBuff(pgps, true),
                               GPS_RX_BUF_SIZE);
}

void gps_RxCpltCallback(void *gps) { gps_rxEventCallback(gps, 0); }

bool gps_newData(GpsCtrl_s *gps) {
  if (gps->data_available) {
    gps_processData(gps);
    return true;
  }
  return false;
}

void gps_init(GpsCtrl_s *gps, UART_HandleTypeDef *huart, GpsType_e type) {
  gps->gps_uart = huart;
  gps->type = type;

  // Register call backs
  halCallbacks_registerUartRxIdleCallback(gps->gps_uart, gps_rxEventCallback,
                                          gps);
  halCallbacks_registerUartRxCpltCallback(gps->gps_uart, gps_RxCpltCallback,
                                          gps);

  // Tell HAL to receive DMA serial data
  gps->firstBuf = true;
  HAL_UARTEx_ReceiveToIdle_DMA(gps->gps_uart, (uint8_t *)gps->rx_firstBuff,
                               GPS_RX_BUF_SIZE);

  if (gps->type == GPS_TYPE_UBLOX) {
    uint8_t nmea[16] = {0xB5,                    // CFG-MSG Header
                        0x62, 0x06, 0x01, 0x08,  // Payload length
                        0x00, 0xF0,              // ZDA
                        0x08, 0x01,              // Enable on I2C
                        0x01,                    // Enable on UART1
                        0x00,                    // Disable on UART2
                        0x01,                    // Enable on USB
                        0x01,                    // Enable on SPI
                        0x00,                    // Always 0x00
                        0x0B,                    // Checksum
                        0x6B};
    // Transmit configuration over UART
    HAL_UART_Transmit(gps->gps_uart, nmea, sizeof(nmea), GPS_UART_TIMEOUT_MS);
  }
}

#endif  // #if HAS_DEV(GPS_STD) || HAS_DEV(GPS_UBLOX)
