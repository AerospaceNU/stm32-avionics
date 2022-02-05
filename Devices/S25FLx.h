/*
 * S25FLx2.h
 */

#ifndef S25FLX_H_
#define S25FLX_H_

#include "board_config.h"

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>
#include <stdbool.h>


// #define USE_S25FLx_DMA
// NOTE: DMA is not yet functional. Leave the define commented and remove this comment when DMA works
// TODO: Make DMA functional

typedef struct {
	SPI_HandleTypeDef *hspi;
	GPIO_TypeDef *csPort;
	uint16_t csPin;
	uint32_t flashSizeBytes;
	bool bWIP; // Write in progress
#ifdef USE_S25FLx_DMA
	bool bTxComplete;
	bool bRxComplete;
#endif
} S25FLXCtrl_t;

void S25FLX_init(S25FLXCtrl_t *s25flx, SPI_HandleTypeDef *hspi, GPIO_TypeDef *csPort, uint16_t csPin, uint32_t flashSizeBytes);
bool S25FLX_read_start(S25FLXCtrl_t *s25flx, uint32_t startLoc, uint32_t numBytes, uint8_t *pData);
bool S25FLX_write_start(S25FLXCtrl_t *s25flx, uint32_t startLoc, uint32_t numBytes, uint8_t *data);
bool S25FLX_erase_sector_start(S25FLXCtrl_t *s25flx, uint32_t sectorNum);
bool S25FLX_erase_chip_start(S25FLXCtrl_t *s25flx);
#ifdef USE_S25FLx_DMA
bool S25FLX_is_read_complete(const S25FLXCtrl_t *s25flx);
#endif
bool S25FLX_is_write_complete(S25FLXCtrl_t *s25flx);
bool S25FLX_is_erase_complete(S25FLXCtrl_t *s25flx);

#ifdef __cplusplus
}
#endif

#endif /* S25FLX_H_ */
