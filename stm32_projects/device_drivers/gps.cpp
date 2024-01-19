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
    case MINMEA_SENTENCE_GGA: {
      struct minmea_sentence_gga frame1;
      if (minmea_parse_gga(&frame1, line)) {
        if (!isnan(minmea_tofloat(&frame1.latitude)) &&
            !isnan(minmea_tofloat(&frame1.longitude)) &&
            !isnan(minmea_tofloat(&frame1.altitude)) &&
            !isnan(minmea_tofloat(&frame1.hdop))) {
          gps->data.generalData.latitude =
              ddmm_to_dddd(minmea_tofloat(&frame1.latitude));
          gps->data.generalData.longitude =
              ddmm_to_dddd(minmea_tofloat(&frame1.longitude));
          gps->data.generalData.altitude = minmea_tofloat(&frame1.altitude);
          gps->data.generalData.fixQuality =
              static_cast<uint8_t>(frame1.fix_quality);
          gps->data.generalData.satsTracked =
              static_cast<uint8_t>(frame1.satellites_tracked);
          gps->data.generalData.hdop = minmea_tofloat(&frame1.hdop);
        }
      }
      break;
    }
    case MINMEA_SENTENCE_ZDA: {
      struct minmea_sentence_zda frame6;
      if (minmea_parse_zda(&frame6, line)) {
        gps->data.timeData.hours = frame6.time.hours;
        gps->data.timeData.minutes = frame6.time.minutes;
        gps->data.timeData.seconds = frame6.time.seconds;
        gps->data.timeData.microseconds = frame6.time.microseconds;
        gps->data.timeData.day = frame6.date.day;
        gps->data.timeData.month = frame6.date.month;
        gps->data.timeData.year = frame6.date.year;

        struct timespec ts;
        memset(&ts, 0, sizeof(ts));

        const struct minmea_date date = {gps->data.timeData.day,
                                         gps->data.timeData.month,
                                         gps->data.timeData.year};
        const struct minmea_time time = {
            gps->data.timeData.hours, gps->data.timeData.minutes,
            gps->data.timeData.seconds, gps->data.timeData.microseconds};

        if (!minmea_gettime(&ts, &date, &time) &&
            gps->data.timeData.year > GPS_MIN_VALID_YEAR &&
            gps->data.timeData.year < GPS_MAX_VALID_YEAR) {
          gps->data.timeData.timestamp = ts.tv_sec;
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
  GpsCtrl_s *pgps = static_cast<GpsCtrl_s *>(gps);
  pgps->data_available = true;

  pgps->lastBufferedSize = Size;

  // Swap the buffer
  pgps->firstBuf = !pgps->firstBuf;

  // Reconfigure the UART
  HAL_UARTEx_ReceiveToIdle_DMA(
      pgps->gps_uart,
      reinterpret_cast<uint8_t *>(gps_getActiveBuff(pgps, true)),
      GPS_RX_BUF_SIZE);
}

void gps_RxCpltCallback(void *gps) {
  gps_rxEventCallback(gps, GPS_RX_BUF_SIZE);
}

bool gps_newData(GpsCtrl_s *gps) {
  if (gps->data_available) {
    gps_processData(gps);
    return true;
  }
  return false;
}

void gps_addUbxChecksum(uint8_t *data, int len) {
  uint8_t CK_A = 0, CK_B = 0;
  for (int i = 2; i < len - 2; i++) {
    CK_A = CK_A + data[i];
    CK_B = CK_B + CK_A;
  }
  data[len - 2] = CK_A;
  data[len - 1] = CK_B;
}

void gps_setMessagesUsed(GpsCtrl_s *gps) {
  // UBX->CFG->MSG

  uint8_t ubloxBuff[] = {
      // Header
      0xB5,  // Sync
      0x62,  // Sync
      0x06,  // Class
      0x01,  // ID
      // Length
      0x08,
      0x00,
      // Type: ZDA
      0xF0,
      0x08,
      // Send rate for IO ports
      0x01,  // Enable on I2C
      0x01,  // Enable on UART1
      0x00,  // Disable on UART2
      0x01,  // Enable on USB
      0x01,  // Enable on SPI
      0x00,  // Always 0x00
      // Checksum
      0x00,
      0x00,
  };

  // Leave on
  /*
   * GGA
   */
  ubloxBuff[7] = 0x00;
  gps_addUbxChecksum(ubloxBuff, sizeof(ubloxBuff));
  HAL_UART_Transmit(gps->gps_uart, ubloxBuff, sizeof(ubloxBuff),
                    GPS_UART_TIMEOUT_MS);

  /*
   * ZDA
   */
  ubloxBuff[7] = 0x08;
  ubloxBuff[8] = 0x0F;
  ubloxBuff[9] = 0x0F;
  ubloxBuff[10] = 0x00;
  ubloxBuff[11] = 0x0F;
  ubloxBuff[12] = 0x0F;
  gps_addUbxChecksum(ubloxBuff, sizeof(ubloxBuff));
  HAL_UART_Transmit(gps->gps_uart, ubloxBuff, sizeof(ubloxBuff),
                    GPS_UART_TIMEOUT_MS);

  // Turn off
  /*
   * RMC
   */
  ubloxBuff[7] = 0x04;
  ubloxBuff[8] = 0x00;
  ubloxBuff[9] = 0x00;
  ubloxBuff[10] = 0x00;
  ubloxBuff[11] = 0x00;
  ubloxBuff[12] = 0x00;
  gps_addUbxChecksum(ubloxBuff, sizeof(ubloxBuff));
  HAL_UART_Transmit(gps->gps_uart, ubloxBuff, sizeof(ubloxBuff),
                    GPS_UART_TIMEOUT_MS);

  /*
   * VTG
   */
  ubloxBuff[7] = 0x05;
  gps_addUbxChecksum(ubloxBuff, sizeof(ubloxBuff));
  HAL_UART_Transmit(gps->gps_uart, ubloxBuff, sizeof(ubloxBuff),
                    GPS_UART_TIMEOUT_MS);

  /*
   * GSV
   */
  ubloxBuff[7] = 0x03;
  gps_addUbxChecksum(ubloxBuff, sizeof(ubloxBuff));
  HAL_UART_Transmit(gps->gps_uart, ubloxBuff, sizeof(ubloxBuff),
                    GPS_UART_TIMEOUT_MS);

  /*
   * GSA
   */
  ubloxBuff[7] = 0x02;
  gps_addUbxChecksum(ubloxBuff, sizeof(ubloxBuff));
  HAL_UART_Transmit(gps->gps_uart, ubloxBuff, sizeof(ubloxBuff),
                    GPS_UART_TIMEOUT_MS);

  /*
   * GLL
   */
  ubloxBuff[7] = 0x01;
  gps_addUbxChecksum(ubloxBuff, sizeof(ubloxBuff));
  HAL_UART_Transmit(gps->gps_uart, ubloxBuff, sizeof(ubloxBuff),
                    GPS_UART_TIMEOUT_MS);
}

void gps_setRate(GpsCtrl_s *gps, uint16_t rate) {
  // UBX->CFG->RATE
  uint8_t ubloxBuff[14] = {
      // Header
      0xB5,  // Sync
      0x62,  // Sync
      0x06,  // Class
      0x08,  // ID
      // Length
      0x06,
      0x00,
      // measRate
      static_cast<uint8_t>(rate & 0xFF),
      static_cast<uint8_t>(rate >> 8),
      // navRate
      0x01,
      0x00,
      // timeRef
      0x01,
      0x00,
      // Checksum
      0x00,
      0x00,
  };

  // Compute and add checksum
  gps_addUbxChecksum(ubloxBuff, sizeof(ubloxBuff));

  // Send data
  HAL_UART_Transmit(gps->gps_uart, ubloxBuff, sizeof(ubloxBuff),
                    GPS_UART_TIMEOUT_MS);
}

void gps_enable4g(GpsCtrl_s *gps) {
  // UBX->CFG->RATE
  uint8_t ubloxBuff[36 + 2 + 4 + 2] = {
      // Header
      0xB5,  // Sync
      0x62,  // Sync
      0x06,  // Class
      0x24,  // ID
      // Length
      0x24, 0x00,
      // Change mast
      0b00000001, 0x00,
      // dynModel
      0x08, 0x00,
      // Unused + Checksum
  };

  // Compute and add checksum
  gps_addUbxChecksum(ubloxBuff, sizeof(ubloxBuff));

  // Send data
  HAL_UART_Transmit(gps->gps_uart, ubloxBuff, sizeof(ubloxBuff),
                    GPS_UART_TIMEOUT_MS);
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
  HAL_UARTEx_ReceiveToIdle_DMA(gps->gps_uart,
                               reinterpret_cast<uint8_t *>(gps->rx_firstBuff),
                               GPS_RX_BUF_SIZE);

  if (gps->type == GPS_TYPE_UBLOX) {
    gps_enable4g(gps);
    gps_setMessagesUsed(gps);
    gps_setRate(gps, 333);
  }
}

float ddmm_to_dddd(float ddmm) {
  int degrees = abs(ddmm / 100);
  float minutes = abs(fmod(ddmm, 100));
  return copysign(degrees + (minutes / 60.), ddmm);
}

#endif  // #if HAS_DEV(GPS_STD) || HAS_DEV(GPS_UBLOX)
