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

typedef struct {
  SPI_HandleTypeDef *hspi;
  GPIO_TypeDef *csPort;
  uint16_t csPin;
  uint32_t flashSizeBytes;
  bool bWIP;  // Write in progress
} FlashMb85rsxCtrl_s;

void flashMb85rsx_init(FlashMb85rsxCtrl_s *s25flx, SPI_HandleTypeDef *hspi,
                      GPIO_TypeDef *csPort, uint16_t csPin,
                      uint32_t flashSizeBytes);
bool flashMb85rsx_readStart(FlashMb85rsxCtrl_s *s25flx, uint32_t startLoc,
                           uint32_t numBytes, uint8_t *pData);
bool flashMb85rsx_writeStart(FlashMb85rsxCtrl_s *s25flx, uint32_t startLoc,
                            uint32_t numBytes, uint8_t *data);
#ifdef USE_S25FLx_DMA
bool flashMb85rsx_isReadComplete(const FlashMb85rsxCtrl_s *s25flx);
#endif
bool flashMb85rsx_isWriteComplete(FlashMb85rsxCtrl_s *s25flx);
bool flashMb85rsx_isEraseComplete(FlashMb85rsxCtrl_s *s25flx);

#ifdef __cplusplus
}
#endif

#endif  // DEVICES_FLASH_S25FLX_H_
