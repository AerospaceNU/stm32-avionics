/*
 * flash_s25flx.h
 */

#ifndef STM32_PROJECTS_DEVICE_DRIVERS_FLASH_MB85RSX_H_
#define STM32_PROJECTS_DEVICE_DRIVERS_FLASH_MB85RSX_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "board_config_common.h"
#include "spi_driver.h"

typedef struct __attribute__((packed)) {
  uint8_t unused2 : 1;
  bool wel : 1;  // Cannot be written

  // If both bp0 and bp1 are set, 0x0000 thru 0x1fff is protected
  bool bp0 : 1;  // 0x1800 thru 0x1fff (upper quarter) protected?
  bool bp1 : 1;  // 0x1000 thru 0x1fff (upper half) protected?

  uint8_t unused : 3;
  bool wpen : 1;  // ignored
} FlashMb85rsxStatusReg_s;

typedef struct {
  SpiCtrl_t spi;
  uint32_t flashSizeBytes;
} FlashMb85rsxCtrl_s;

void flashMb85rsx_init(FlashMb85rsxCtrl_s* s25flx, SPI_HandleTypeDef* hspi,
                       GPIO_TypeDef* csPort, uint16_t csPin);
bool flashMb85rsx_readStart(FlashMb85rsxCtrl_s* s25flx, uint32_t startLoc,
                            uint32_t numBytes, uint8_t* pData);
bool flashMb85rsx_writeStart(FlashMb85rsxCtrl_s* s25flx, uint32_t startLoc,
                             uint32_t numBytes, uint8_t* data);
#ifdef USE_S25FLx_DMA
bool flashMb85rsx_isReadComplete(const FlashMb85rsxCtrl_s* s25flx);
#endif
bool flashMb85rsx_isWriteComplete(FlashMb85rsxCtrl_s* s25flx);
bool flashMb85rsx_isEraseComplete(FlashMb85rsxCtrl_s* s25flx);

#ifdef __cplusplus
}
#endif

#endif  // STM32_PROJECTS_DEVICE_DRIVERS_FLASH_MB85RSX_H_
