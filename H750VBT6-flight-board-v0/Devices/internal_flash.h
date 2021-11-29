/*
 * internal_flash.h
 */

#ifndef INTERNAL_FLASH_H_
#define INTERNAL_FLASH_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "stm32h7xx_hal.h"
#include <stdbool.h>

#define FLASH_START_ADDRESS      0x08020000
#define FLASH_END_ADDRESS        0x081FFFFF
#define MAX_FLASH_ADDRESS        FLASH_END_ADDRESS - FLASH_START_ADDRESS

bool internal_flash_write(uint32_t RelFlashAddress, uint8_t *data, uint32_t numBytes);

bool internal_flash_read(uint32_t RelFlashAddress, uint8_t *pData, uint32_t numBytes);

#ifdef __cplusplus
}
#endif

#endif /* INTERNAL_FLASH_H_ */
