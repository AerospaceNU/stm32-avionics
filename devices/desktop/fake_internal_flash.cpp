//
// Created by matth on 3/25/2022.
//

#include "fake_internal_flash.h"

#include <stdbool.h>
#include <stdint.h>

#include "file_backed_flash.h"
#include "internal_flash.h"

namespace detail {
FileBackedFlash *internalFlash;
}

void internalFlash_sim_setFilePointer(FileBackedFlash *ptr) {
  detail::internalFlash = ptr;
}

bool internalFlash_write(uint32_t RelFlashAddress, uint8_t *data,
                         uint32_t numBytes) {
  if (!detail::internalFlash) return false;
  return detail::internalFlash->WriteStart(RelFlashAddress, numBytes, data);
}

bool internalFlash_read(uint32_t RelFlashAddress, uint8_t *pData,
                        uint32_t numBytes) {
  if (!detail::internalFlash) return false;
  return detail::internalFlash->ReadStart(RelFlashAddress, numBytes, pData);
}
