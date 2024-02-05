/*
 * internal_flash.c
 */

#include "internal_flash.h"

#include <string.h>

#include "board_config_common.h"

#define FLASH_TIMEOUT_VALUE 50000U /* 50 s */
#define FLASH_BYTE_INCREMENT 32
#define ERASED 0xFF

/*
 * Writes to the internal flash of the H750VBT6
 * @param RelFlashAddress - Relative address to write to, where 0 means the
 * defined start of writable internal flash
 * @param data - Array of uint8_t to write
 * @param numBytes - Number of bytes to be written from data
 */
bool internalFlash_write(uint32_t RelFlashAddress, uint8_t* data,
                         uint32_t numBytes) {
  return true;
}

/*
 * Reads from the internal flash of the H750VBT6
 * @param RelFlashAddress - Relative address to read from, where 0 means the
 * defined start of readable internal flash
 * @param pData - Array of uint8_t to read into
 * @param numBytes - Number of bytes to be read from flash
 */
bool internalFlash_read(uint32_t RelFlashAddress, uint8_t* pData,
                        uint32_t numBytes) {
  return false;
}
