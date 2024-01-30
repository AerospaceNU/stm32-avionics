#include "gps.h"

//#if HAS_DEV(GPS_STD) || HAS_DEV(GPS_UBLOX)

#include <string.h>

#include "hal_callbacks.h"
#include "hardware_manager.h"
#include "custom_stm.h"

#define INPUT_BUFFER_SIZE 512
static uint8_t bleRxBuffer[INPUT_BUFFER_SIZE];
static CircularBuffer_s bleRxCircBuffer;

void cli_bleCallback(void*, uint8_t* pdata, size_t len) {
  for (size_t i = 0; i < len; i++) {
    cb_enqueue(&bleRxCircBuffer, (unknownPtr_t)pdata + i);
  }
}

void bluetooth_uart_init() {
	  cb_init(&bleRxCircBuffer, (unknownPtr_t)bleRxBuffer, sizeof(bleRxBuffer), 1);
	  halCallbacks_registerBluetoothRxCallback(CUSTOM_STM_RX_WRITE_EVT,
	                                           cli_bleCallback, NULL);
}

CircularBuffer_s *bleUart_getRxBuffer() { return &bleRxCircBuffer; }

//#endif  // #if HAS_DEV(GPS_STD) || HAS_DEV(GPS_UBLOX)
