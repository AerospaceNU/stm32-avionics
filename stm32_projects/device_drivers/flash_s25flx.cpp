/*
 * flash_s25flx.c
 */

#include "flash_s25flx.h"

#if HAS_DEV(FLASH_S25FLX)

#include <string.h>

#include "hal_callbacks.h"

// Commands
#define WRITE_ENABLE_CMD 0x06
#define WRITE_DISABLE_CMD 0x04
#define READ_STAT_REG_CMD 0x05
#define READ_DATA_CMD 0x13
#define PAGE_PROGRAM_CMD 0x12
#define SECTOR_ERASE_CMD 0xDC
#define CHIP_ERASE_CMD 0xC7

// Timeouts
#define SPI_TX_RX_TIMEOUT_MS \
  100  // For short transmit-receives that don't require DMA

// Flash properties
#define PAGE_SIZE_BYTES 512
#define SECTOR_SIZE_BYTES 262144

static void csPull(const FlashS25flxCtrl_s *s25flx, GPIO_PinState direction) {
  HAL_GPIO_WritePin(s25flx->csPort, s25flx->csPin, direction);
}

static bool writeDisable(FlashS25flxCtrl_s *s25flx) {
  csPull(s25flx, GPIO_PIN_RESET);
  uint8_t tx = WRITE_DISABLE_CMD;
  // Write-disable command is too short to require DMA
  bool bSuccess =
      HAL_SPI_Transmit(s25flx->hspi, &tx, 1, SPI_TX_RX_TIMEOUT_MS) == HAL_OK;
  csPull(s25flx, GPIO_PIN_SET);
  return bSuccess;
}

static bool writeEnable(FlashS25flxCtrl_s *s25flx) {
  csPull(s25flx, GPIO_PIN_RESET);
  uint8_t tx = WRITE_ENABLE_CMD;
  // Write-enable command is too short to require DMA
  bool bSuccess =
      HAL_SPI_Transmit(s25flx->hspi, &tx, 1, SPI_TX_RX_TIMEOUT_MS) == HAL_OK;
  csPull(s25flx, GPIO_PIN_SET);
  return bSuccess;
}

static void checkWriteInProgress(FlashS25flxCtrl_s *s25flx) {
  csPull(s25flx, GPIO_PIN_RESET);
  uint8_t tx[] = {READ_STAT_REG_CMD, 0};
  uint8_t rx[] = {0, 0};
  // Read status register once
  bool bSuccess = HAL_SPI_TransmitReceive(s25flx->hspi, tx, rx, 2,
                                          SPI_TX_RX_TIMEOUT_MS) == HAL_OK;
  if (bSuccess)
    s25flx->bWIP =
        ((rx[1] & 0x01) == 0x01);  // Write in progress is LSB of status
                                   // register (1 = busy, 0 = not busy)
  else
    s25flx->bWIP = true;  // If status register can't be read, assume the
                          // worst-case scenario: flash is busy
  csPull(s25flx, GPIO_PIN_SET);
}

#ifdef USE_S25FLx_DMA
static void spiTxCpltCallback(void *s25flx) {
  // Input must be void to use this function as a callback, but this is assumed
  // to be a S25FLXCtrl_t type
  FlashS25flxCtrl_t *ps25flx = (FlashS25flxCtrl_s *)s25flx;
  ps25flx->bTxComplete = true;
  // There is no instance where a complete DMA TX alone would not be followed by
  // a CS pull high to end a command. Therefore, it makes sense to do that here
  // instead of making higher-level code responsible to call some cleanup
  // function
  csPull(s25flx, GPIO_PIN_SET);
}

static void spiRxCpltCallback(void *s25flx) {
  // Input must be void to use this function as a callback, but this is assumed
  // to be a S25FLXCtrl_t type
  FlashS25flxCtrl_t *ps25flx = (FlashS25flxCtrl_s *)s25flx;
  ps25flx->bRxComplete = true;
  // There is no instance where a complete DMA RX would not be followed by a CS
  // pull high to end a command. Therefore, it makes sense to do that here
  // instead of making higher-level code responsible to call some cleanup
  // function.
  csPull(s25flx, GPIO_PIN_SET);
}
#endif

void flashS25flx_init(FlashS25flxCtrl_s *s25flx, SPI_HandleTypeDef *hspi,
                      GPIO_TypeDef *csPort, uint16_t csPin,
                      uint32_t flashSizeBytes) {
  // Set struct properties
  s25flx->hspi = hspi;
  s25flx->csPort = csPort;
  s25flx->csPin = csPin;
  s25flx->flashSizeBytes = flashSizeBytes;
  s25flx->bWIP = false;
#ifdef USE_S25FLx_DMA
  s25flx->bRxComplete = true;
  s25flx->bTxComplete = true;

  // Register callbacks for HAL SPI transmit-receive completions
  halCallbacks_registerSpiTxCpltCallback(s25flx->hspi, spiTxCpltCallback,
                                         s25flx);
  halCallbacks_registerSpiTxRxCpltCallback(s25flx->hspi, spiRxCpltCallback,
                                           s25flx);
#endif

  // Ensure CS is pulled high
  HAL_GPIO_WritePin(s25flx->csPort, s25flx->csPin, GPIO_PIN_SET);
}

bool flashS25flx_readStart(FlashS25flxCtrl_s *s25flx, uint32_t startLoc,
                           uint16_t numBytes, uint8_t *pData) {
  // Check for valid parameters
  if (startLoc + numBytes > s25flx->flashSizeBytes || pData == NULL ||
      s25flx->bWIP
#ifdef USE_S25FLx_DMA
      || !s25flx->bRxComplete
#endif
  )
    return false;

  // Transmit read command and location
  csPull(s25flx, GPIO_PIN_RESET);
  uint8_t txBuf1[5] = {READ_DATA_CMD,
                       static_cast<uint8_t>((startLoc >> 24) & 0xff),
                       static_cast<uint8_t>((startLoc >> 16) & 0xff),
                       static_cast<uint8_t>(startLoc >> 8 & 0xff),
                       static_cast<uint8_t>(startLoc & 0xff)};
  // No need to transmit 5-byte read command with DMA since it should occur in
  // very short period of time
  if (HAL_SPI_Transmit(s25flx->hspi, txBuf1, 5, SPI_TX_RX_TIMEOUT_MS) !=
      HAL_OK) {
    csPull(s25flx, GPIO_PIN_SET);
    return false;
  }

  // Read into given buffer pData
  uint8_t txBuf2[numBytes];  // NOLINT
  memset(txBuf2, 0, numBytes);
#ifdef USE_S25FLx_DMA
  s25flx->bRxComplete = false;
  if (HAL_SPI_TransmitReceive_DMA(s25flx->hspi, txBuf2, pData, numBytes) !=
      HAL_OK) {
    s25flx->bRxComplete = true;
#else
  if (HAL_SPI_TransmitReceive(s25flx->hspi, txBuf2, pData, numBytes,
                              SPI_TX_RX_TIMEOUT_MS) != HAL_OK) {
#endif
    csPull(s25flx, GPIO_PIN_SET);
    return false;
  }

#ifndef USE_S25FLx_DMA
  // Pull CS high if not using DMA because read is complete
  csPull(s25flx, GPIO_PIN_SET);
#endif
  // If using DMA, don't pull CS high because transmit-receive still needs to
  // occur on hardware communication end CS should be pulled high when receive
  // is complete

  return true;
}

bool flashS25flx_writeStart(FlashS25flxCtrl_s *s25flx, uint32_t startLoc,
                            uint16_t numBytes, uint8_t *data) {
  // Only allows writing to 1 page at a time

  // Check for valid parameters
  if (startLoc + numBytes > s25flx->flashSizeBytes ||
      startLoc / PAGE_SIZE_BYTES !=
          (startLoc + numBytes - 1) / PAGE_SIZE_BYTES ||
      data == NULL || s25flx->bWIP
#ifdef USE_S25FLx_DMA
      || !s25flx->bRxComplete
#endif
  )
    return false;

  // Assume data is erased beforehand.
  // This is up to higher-level code to manage since writing over existing data
  // won't cause an error.

  // Enable writing to flash
  if (!writeEnable(s25flx)) return false;

  // Set up data transfer
  s25flx->bWIP = true;  // Assume write is in progress for flash until status
                        // register reads otherwise
#ifdef USE_S25FLx_DMA
  s25flx->bTxComplete = false;  // Assume DMA transmission hasn't been completed
                                // until callback has been called
#endif
  csPull(s25flx, GPIO_PIN_RESET);

  // Fill in TX buffer for page program command and send
  uint8_t txBuf[5];
  bool success = false;
  txBuf[0] = PAGE_PROGRAM_CMD;
  for (int i = 1; i < 5; i++) {
    txBuf[i] = (startLoc >> (8 * (4 - i))) & 0xFF;
  }
  success =
      HAL_SPI_Transmit(s25flx->hspi, txBuf, 5, SPI_TX_RX_TIMEOUT_MS) == HAL_OK;

  // Perform transmit
#ifdef USE_S25FLx_DMA
  if (HAL_SPI_Transmit_DMA(s25flx->hspi, data, numBytes) != HAL_OK ||
      !success) {
    s25flx->bTxComplete = true;
#else
  if (HAL_SPI_Transmit(s25flx->hspi, data, numBytes, SPI_TX_RX_TIMEOUT_MS) !=
          HAL_OK ||
      !success) {
#endif
    s25flx->bWIP = false;
    csPull(s25flx, GPIO_PIN_SET);
    writeDisable(s25flx);
    return false;
  }

#ifndef USE_S25FLx_DMA
  // Pull CS high if not using DMA because write transmission is complete
  csPull(s25flx, GPIO_PIN_SET);
#endif
  // If using DMA, don't pull CS high since transmit of data still needs to
  // occur on hardware end CS should be pulled high when transmit is complete
  // (before write is complete though

  return true;
}

bool flashS25flx_eraseSectorStart(FlashS25flxCtrl_s *s25flx,
                                  uint32_t sectorNum) {
  // Check for valid parameters
  if (sectorNum * SECTOR_SIZE_BYTES >= s25flx->flashSizeBytes) return false;

  // Enable writing to flash (also necessary for erasing)
  if (!writeEnable(s25flx)) return false;

  // Prep erase command
  s25flx->bWIP = true;  // Assume write (erase counts, too) is in progress for
                        // flash until status reg reads otherwise
  csPull(s25flx, GPIO_PIN_RESET);

  // Fill in TX buffer
  uint8_t txBuf[5];
  txBuf[0] = SECTOR_ERASE_CMD;
  for (int i = 1; i < 5; i++) {
    txBuf[i] = ((sectorNum * SECTOR_SIZE_BYTES) >> (8 * (4 - i))) & 0xFF;
  }

  // Perform transmit. Transmit is short enough where DMA doesn't speed things
  // up
  if (HAL_SPI_Transmit(s25flx->hspi, txBuf, 5, SPI_TX_RX_TIMEOUT_MS) !=
      HAL_OK) {
    csPull(s25flx, GPIO_PIN_SET);
    writeDisable(s25flx);
    s25flx->bWIP = false;
    return false;
  }

  // Latch the erase command so it starts
  csPull(s25flx, GPIO_PIN_SET);

  return true;
}

bool flashS25flx_eraseChipStart(FlashS25flxCtrl_s *s25flx) {
  // Enable writing to flash (also necessary for erasing)
  if (!writeEnable(s25flx)) return false;

  // Perform full erase command. Short enough to avoid DMA
  s25flx->bWIP = true;  // Assume write (erase counts, too) is in progress for
                        // flash until status reg reads otherwise
  csPull(s25flx, GPIO_PIN_RESET);
  uint8_t txByte = CHIP_ERASE_CMD;
  if (HAL_SPI_Transmit(s25flx->hspi, &txByte, 1, SPI_TX_RX_TIMEOUT_MS) !=
      HAL_OK) {
    csPull(s25flx, GPIO_PIN_SET);
    writeDisable(s25flx);
    s25flx->bWIP = false;
    return false;
  }

  // Latch the full erase command so it starts
  csPull(s25flx, GPIO_PIN_SET);

  return true;
}

#ifdef USE_S25FLx_DMA
bool flashS25flx_isReadComplete(const FlashS25flxCtrl_s *s25flx) {
  return s25flx->bRxComplete;
}
#endif

bool flashS25flx_isWriteComplete(FlashS25flxCtrl_s *s25flx) {
  checkWriteInProgress(s25flx);
  if (s25flx->bWIP) {
    HAL_Delay(1);
  }
  // This delay is here because checking write in progress too quickly causes
  // issues There should be a way to check if ready instead of delaying, but no
  // solution has been found yet
  return !s25flx->bWIP;
}

bool flashS25flx_isEraseComplete(FlashS25flxCtrl_s *s25flx) {
  // Same functionality as write, but different public function name is clearer
  // for programmers at higher levels
  return flashS25flx_isWriteComplete(s25flx);
}

#endif  // HAS_DEV(FLASH_S25FLX)
