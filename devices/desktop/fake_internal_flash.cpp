//
// Created by matth on 3/25/2022.
//

#include <stdbool.h>
#include <stdint.h>

#include "file_backed_flash.h"
#include "internal_flash.h"

extern FileBackedFlash *internalFlash;

bool internalFlash_write(uint32_t RelFlashAddress, uint8_t *data,
                         uint32_t numBytes) {
  return internalFlash->writeStart(RelFlashAddress, numBytes, data);
}

bool internalFlash_read(uint32_t RelFlashAddress, uint8_t *pData,
                        uint32_t numBytes) {
  return internalFlash->readStart(RelFlashAddress, numBytes, pData);
}
