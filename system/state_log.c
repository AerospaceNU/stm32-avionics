/*
 * state_log.c
 */
#include "state_log.h"

#include <stdint.h>

#include "data_log.h"
#include "filters.h"
#include "hardware_manager.h"
#include "internal_flash.h"
#include "pyro_manager.h"

#define STATE_START_ADDRESS \
  0x00000000;  // Starting flash location for the state log, relative to start
               // of internal flash

uint32_t writeAddress = STATE_START_ADDRESS;
uint32_t readAddress = STATE_START_ADDRESS;
uint8_t curRead;
uint8_t prevRead;

void state_log_reload_flight() {
  data_log_load_last_stored_flight_metadata();  // Load the previous flight
                                                // metadata into the data log
  FlightMetadata oldMetadataPacket = *data_log_get_flight_metadata();

  // Load in recovered values
  filterSetPressureRef(oldMetadataPacket.pressureRef);
  PyroManager_Init();
  PyroManager_SetApogeeTime(HM_Millis());
  PyroManager_SetPyroFireStatus(~oldMetadataPacket.pyroFireStatus);

  // Assign a new flight
  data_log_assign_flight();

  // Set new metadata to values in old packet
  *data_log_get_flight_metadata() = oldMetadataPacket;
  data_log_write_flight_metadata();
}

void state_log_write(int currentState) {
  uint8_t stateWriteBuffer;  // Buffer to store the byte that needs to be
                             // written to flash
  uint8_t stateReadBuffer;
  while (1) {
    internal_flash_read(writeAddress, &stateReadBuffer, 1);
    curRead = stateReadBuffer;  // Read from the address
    if (curRead == 255) {       // Find the first empty address
      break;
    }
    writeAddress += 32;  // Increase the write address by 32 bytes to account
                         // for the flash write size
    if (writeAddress > MAX_FLASH_ADDRESS) {
      return;  // Return if the address is too high
    }
  }

  // If we're at the last writeable address, write 0xEE to indicate
  // that no state should be resumed, because we can't be sure
  // that the state it would resume is correct
  if (writeAddress == MAX_FLASH_ADDRESS - 31) {
    stateWriteBuffer = (uint8_t)(0xEE);
  } else {
    stateWriteBuffer = (uint8_t)(currentState);  // Otherwise write the state
  }

  internal_flash_write(writeAddress, &stateWriteBuffer, 1);
}

void state_log_write_complete() {
  state_log_write(0xEE);  // Write 0xEE to the flash to indicate that the flight
                          // ended and nothing should be resumed
}

int state_log_read() {
  uint8_t stateReadBuffer;
  readAddress = STATE_START_ADDRESS;
  internal_flash_read(readAddress, &stateReadBuffer, 1);
  prevRead = stateReadBuffer;

  if (prevRead == 255) {  // If we immediately read 255, the state log is empty,
                          // so return -1 for no state to resume
    return -1;
  }

  while (1) {
    internal_flash_read(readAddress, &stateReadBuffer, 1);
    curRead = stateReadBuffer;
    readAddress += 32;

    // If we reach an empty state or are going to read past the last address
    if (curRead == 255 || readAddress > MAX_FLASH_ADDRESS) {
      if (prevRead == 0xEE) {  // Nothing to resume if the last data is 0xEE
        return -1;
      }
      return prevRead;  // Otherwise return the state id to resume
    }
    prevRead = curRead;
  }
}
