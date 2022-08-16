/*
 * flash_s25flx.h
 */

#ifndef DEVICES_FLASH_S25FLX_H_
#define DEVICES_FLASH_S25FLX_H_

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
  SPI_HandleTypeDef *hspi;
  GPIO_TypeDef *csPort;
  uint16_t csPin;
  uint32_t flashSizeBytes;
  bool bWIP;  // Write in progress
#ifdef USE_S25FLx_DMA
  bool bTxComplete;
  bool bRxComplete;
#endif
} FlashS25flxCtrl_t;

void S25FLX_init(FlashS25flxCtrl_t *s25flx, SPI_HandleTypeDef *hspi,
                 GPIO_TypeDef *csPort, uint16_t csPin, uint32_t flashSizeBytes);
bool S25FLX_read_start(FlashS25flxCtrl_t *s25flx, uint32_t startLoc,
                       uint32_t numBytes, uint8_t *pData);
bool S25FLX_write_start(FlashS25flxCtrl_t *s25flx, uint32_t startLoc,
                        uint32_t numBytes, uint8_t *data);
bool S25FLX_erase_sector_start(FlashS25flxCtrl_t *s25flx, uint32_t sectorNum);
bool S25FLX_erase_chip_start(FlashS25flxCtrl_t *s25flx);
#ifdef USE_S25FLx_DMA
bool S25FLX_is_read_complete(const FlashS25flxCtrl_t *s25flx);
#endif
bool S25FLX_is_write_complete(FlashS25flxCtrl_t *s25flx);
bool S25FLX_is_erase_complete(FlashS25flxCtrl_t *s25flx);

#ifdef __cplusplus
}
#endif

#endif  // DEVICES_FLASH_S25FLX_H_
