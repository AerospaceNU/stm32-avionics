#include "data_offload.h"

#include <stdlib.h>

#include "board_config_common.h"
#include "data_log.h"
#include "hardware_manager.h"

#define FLASH_READ_BUF_SIZE 2048

static bool readComplete_ = false;
static uint32_t flightId_ = 0;
uint8_t flashBuf_[FLASH_READ_BUF_SIZE];

// Reads data of current flight ID and transmits it over USB
bool dataOffload_tick() {
  if (!readComplete_) {
    uint32_t bytesRead =
        dataLog_read(flightId_, FLASH_READ_BUF_SIZE, flashBuf_, false);
    if (bytesRead == 0) {
      readComplete_ = true;
    } else {
      hm_usbTransmit(USB_CLI_ID, flashBuf_, (uint16_t)bytesRead);
    }
  }
  return readComplete_;
}

void dataOffload_setFlightId(uint32_t flightId) {
  flightId_ = flightId;
  readComplete_ = false;
  dataLog_read(
      flightId_, 0, NULL,
      true);  // Reset flight in case same flight ID is offloaded twice in a row
}
