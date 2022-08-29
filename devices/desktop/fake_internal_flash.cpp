//
// Created by matth on 3/25/2022.
//

#include <stdbool.h>
#include <stdint.h>

#include "file_backed_flash.h"
#include "internal_flash.h"

extern FileBackedFlash *internalFlash;

bool internal_flash_write(uint32_t RelFlashAddress, uint8_t *data,
                          uint32_t numBytes) {
  return internalFlash->WriteStart(RelFlashAddress, numBytes, data);
}

bool internal_flash_read(uint32_t RelFlashAddress, uint8_t *pData,
                         uint32_t numBytes) {
  return internalFlash->ReadStart(RelFlashAddress, numBytes, pData);
}
