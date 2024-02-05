/*
 * internal_flash.h
 */

#ifndef PLATFORMIO_PROJECTS_LIB_NEW_HAL_INTERNAL_FLASH_H_
#define PLATFORMIO_PROJECTS_LIB_NEW_HAL_INTERNAL_FLASH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#if defined(STM32H750xx)
#define INTERNAL_FLASH_START 0x08020000
#define INTERNAL_FLASH_END 0x080FFFFF
#elif defined(STM32H743xx)
#define INTERNAL_FLASH_START 0x081E0000
#define INTERNAL_FLASH_END 0x081FFFFF
#else
#define INTERNAL_FLASH_START 0x00
#define INTERNAL_FLASH_END 0xDFFFF
#endif  // STM32xxxxxx

#define MAX_FLASH_ADDRESS INTERNAL_FLASH_END - INTERNAL_FLASH_START

bool internalFlash_write(uint32_t RelFlashAddress, uint8_t* data,
                         uint32_t numBytes);

bool internalFlash_read(uint32_t RelFlashAddress, uint8_t* pData,
                        uint32_t numBytes);

#ifdef __cplusplus
}
#endif

#endif  // PLATFORMIO_PROJECTS_LIB_NEW_HAL_INTERNAL_FLASH_H_
