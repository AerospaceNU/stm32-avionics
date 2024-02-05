/*
 * flash_s25flx.h
 */

#ifndef STM32_PROJECTS_DEVICE_DRIVERS_FLASH_S25FLX_H_
#define STM32_PROJECTS_DEVICE_DRIVERS_FLASH_S25FLX_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "board_config_common.h"

// #define USE_S25FLx_DMA
// NOTE: DMA is not yet functional. Leave the define commented and remove this
// comment when DMA works
// TODO: Make DMA functional

typedef struct {
  SPI_HandleTypeDef* hspi;
  GPIO_TypeDef* csPort;
  uint16_t csPin;
  uint32_t flashSizeBytes;
  bool bWIP;  // Write in progress
#ifdef USE_S25FLx_DMA
  bool bTxComplete;
  bool bRxComplete;
#endif
} FlashS25flxCtrl_s;

void flashS25flx_init(FlashS25flxCtrl_s* s25flx, SPI_HandleTypeDef* hspi,
                      GPIO_TypeDef* csPort, uint16_t csPin,
                      uint32_t flashSizeBytes);
bool flashS25flx_readStart(FlashS25flxCtrl_s* s25flx, uint32_t startLoc,
                           uint16_t numBytes, uint8_t* pData);
bool flashS25flx_writeStart(FlashS25flxCtrl_s* s25flx, uint32_t startLoc,
                            uint16_t numBytes, uint8_t* data);
bool flashS25flx_eraseSectorStart(FlashS25flxCtrl_s* s25flx,
                                  uint32_t sectorNum);
bool flashS25flx_eraseChipStart(FlashS25flxCtrl_s* s25flx);
#ifdef USE_S25FLx_DMA
bool flashS25flx_isReadComplete(const FlashS25flxCtrl_s* s25flx);
#endif
bool flashS25flx_isWriteComplete(FlashS25flxCtrl_s* s25flx);
bool flashS25flx_isEraseComplete(FlashS25flxCtrl_s* s25flx);

#ifdef __cplusplus
}
#endif

#endif  // STM32_PROJECTS_DEVICE_DRIVERS_FLASH_S25FLX_H_
