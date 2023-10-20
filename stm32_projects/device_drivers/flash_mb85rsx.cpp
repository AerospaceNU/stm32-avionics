/*
 * flash_s25flx.c
 */

#include "flash_mb85rsx.h"

#if HAS_DEV(FLASH_MB85RSX)

#include <string.h>

#include "hal_callbacks.h"

// Timeouts
#define SPI_TX_RX_TIMEOUT_MS \
  100  // For short transmit-receives that don't require DMA

#define WRITE_ENABLE 0b110
#define WRITE_DISABLE 0b100
#define READ_STATUS_REG 0b101
#define WRITE_STATUS_REG 0b1
#define READ_CMD 0b11
#define WRITE_CMD 0b10

static bool writeDisable(FlashMb85rsxCtrl_s *mb85rsx) {
  uint8_t cmd = WRITE_DISABLE;
  spi_writeArray(&mb85rsx->spi, &cmd, sizeof(cmd));
  return true;  // TODO error checking
}

static bool writeEnable(FlashMb85rsxCtrl_s *mb85rsx) {
  uint8_t cmd = WRITE_ENABLE;
  spi_writeArray(&mb85rsx->spi, &cmd, sizeof(cmd));
  return true;  // TODO error checking
}

static void csPull(const FlashMb85rsxCtrl_s *mb85rsx, GPIO_PinState direction) {
  HAL_GPIO_WritePin(mb85rsx->spi.port, mb85rsx->spi.pin, direction);
}

void flashMb85rsx_init(FlashMb85rsxCtrl_s *mb85rsx, SPI_HandleTypeDef *hspi,
                       GPIO_TypeDef *csPort, uint16_t csPin) {
  // Set struct properties
  mb85rsx->spi.hspi = hspi;
  mb85rsx->spi.port = csPort;
  mb85rsx->spi.pin = csPin;
  mb85rsx->flashSizeBytes = 8192;

  // Ensure CS is pulled high
  HAL_GPIO_WritePin(mb85rsx->spi.port, mb85rsx->spi.pin, GPIO_PIN_SET);

  // Write protection is enabled at power on, but reset JIC
  uint8_t cmd = WRITE_DISABLE;
  spi_writeArray(&mb85rsx->spi, &cmd, sizeof(cmd));

  // disable protection on all blocks
  FlashMb85rsxStatusReg_s reg = {.bp0 = false, .bp1 = false};
  spi_writeRegister(&mb85rsx->spi, WRITE_STATUS_REG, *(uint8_t *)&reg);
}

bool flashMb85rsx_readStart(FlashMb85rsxCtrl_s *mb85rsx, uint32_t startLoc,
                            uint32_t numBytes, uint8_t *pData) {
  // Check for valid parameters
  if ((startLoc + numBytes) > mb85rsx->flashSizeBytes || pData == NULL ||
      startLoc) {
    return false;
  }

  // Transmit read command and location
  csPull(mb85rsx, GPIO_PIN_RESET);

  uint8_t cmd[3] = {READ_CMD, static_cast<uint8_t>((startLoc >> 8) & 0b11111),
                    static_cast<uint8_t>(startLoc & 0xff)};
  HAL_SPI_Transmit(mb85rsx->spi.hspi, cmd, sizeof(cmd), SPI_TX_RX_TIMEOUT_MS);
  HAL_SPI_Receive(mb85rsx->spi.hspi, pData, numBytes, SPI_TX_RX_TIMEOUT_MS);

  // Reset CS pull
  csPull(mb85rsx, GPIO_PIN_SET);

  return true;
}

bool flashMb85rsx_writeStart(FlashMb85rsxCtrl_s *mb85rsx, uint32_t startLoc,
                             uint32_t numBytes, uint8_t *pData) {
  // Check for valid parameters
  if (startLoc + numBytes > mb85rsx->flashSizeBytes || pData == NULL)
    return false;

  // Assume data is erased beforehand.
  // This is up to higher-level code to manage since writing over existing data
  // won't cause an error.

  // Enable writing to flash
  if (!writeEnable(mb85rsx)) return false;

  csPull(mb85rsx, GPIO_PIN_RESET);

  // Fill in TX buffer for WRITE command and send
  uint8_t cmd[3] = {WRITE_CMD, static_cast<uint8_t>((startLoc >> 8) & 0b11111),
                    static_cast<uint8_t>(startLoc & 0xff)};
  bool success = HAL_SPI_Transmit(mb85rsx->spi.hspi, cmd, sizeof(cmd),
                                  SPI_TX_RX_TIMEOUT_MS) == HAL_OK;

  // Perform transmit
  if (HAL_SPI_Transmit(mb85rsx->spi.hspi, pData, numBytes,
                       SPI_TX_RX_TIMEOUT_MS) != HAL_OK ||
      !success) {
    csPull(mb85rsx, GPIO_PIN_SET);
    writeDisable(mb85rsx);
    return false;
  }

  csPull(mb85rsx, GPIO_PIN_SET);

  return true;
}

#endif  // HAS_DEV(FLASH_MB85RSX)
