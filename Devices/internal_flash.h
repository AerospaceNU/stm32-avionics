/*
 * internal_flash.h
 */

#ifndef DEVICES_INTERNAL_FLASH_H_
#define DEVICES_INTERNAL_FLASH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#if (FCB_VERSION == 0)
#define INTERNAL_FLASH_START 0x08020000
#define INTERNAL_FLASH_END 0x080FFFFF
#else
#define INTERNAL_FLASH_START 0x081E0000
#define INTERNAL_FLASH_END 0x081FFFFF
#endif

#define MAX_FLASH_ADDRESS INTERNAL_FLASH_END - INTERNAL_FLASH_START

bool internal_flash_write(uint32_t RelFlashAddress, uint8_t *data,
                          uint32_t numBytes);

bool internal_flash_read(uint32_t RelFlashAddress, uint8_t *pData,
                         uint32_t numBytes);

#ifdef __cplusplus
}
#endif

#endif  // DEVICES_INTERNAL_FLASH_H_
