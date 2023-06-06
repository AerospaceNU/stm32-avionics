#ifndef DESKTOP_PROJECTS_DESKTOP_DEVICES_INTERNAL_FLASH_H_
#define DESKTOP_PROJECTS_DESKTOP_DEVICES_INTERNAL_FLASH_H_

#include <stdint.h>

#define INTERNAL_FLASH_START 0x00
#define INTERNAL_FLASH_END 0xDFFFF

#define MAX_FLASH_ADDRESS INTERNAL_FLASH_END - INTERNAL_FLASH_START

#ifdef __cplusplus
extern "C" {
#endif

bool internalFlash_write(uint32_t RelFlashAddress, uint8_t *data,
                         uint32_t numBytes);

bool internalFlash_read(uint32_t RelFlashAddress, uint8_t *pData,
                        uint32_t numBytes);

#ifdef __cplusplus
}
#endif

#endif  // DESKTOP_PROJECTS_DESKTOP_DEVICES_INTERNAL_FLASH_H_
