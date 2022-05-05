#include "ms5607.h"

#ifdef HAS_MS5607

#define SPI_TIMEOUT_MS 50

/*
 * Initializes MS5607 temperature and pressure sensor
 * with specific MS5607Ctrl_t configuration
 */
bool MS5607_init(MS5607Ctrl_t *altCtrl) {
  bool success = true;
  uint8_t init = 0x1E;  // Initialization hex command for MS5607

  // Pull CS Low
  HAL_GPIO_WritePin(altCtrl->spiconfig.port, altCtrl->spiconfig.pin,
                    GPIO_PIN_RESET);

  // Send the initialization command and delay for 3ms
  HAL_SPI_Transmit(altCtrl->spiconfig.hspi, &init, 1, SPI_TIMEOUT_MS);
  HAL_Delay(3);

  // Pull CS High
  HAL_GPIO_WritePin(altCtrl->spiconfig.port, altCtrl->spiconfig.pin,
                    GPIO_PIN_SET);

  // PROM read
  uint8_t rxBuffer[3];
  uint8_t txBuffer[3];
  uint8_t promRead[6] = {0xA2, 0xA4, 0xA6, 0xA8, 0xAA, 0xAC};
  for (int i = 0; i < 6; i++) {
    txBuffer[0] = promRead[i];
    HAL_GPIO_WritePin(altCtrl->spiconfig.port, altCtrl->spiconfig.pin,
                      GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive(altCtrl->spiconfig.hspi, txBuffer, rxBuffer, 3,
                            SPI_TIMEOUT_MS);
    HAL_GPIO_WritePin(altCtrl->spiconfig.port, altCtrl->spiconfig.pin,
                      GPIO_PIN_SET);
    altCtrl->promVals[i] = rxBuffer[1] << 8 | rxBuffer[2];
    if (altCtrl->promVals[i] == 0xFF || altCtrl->promVals[i] == 0x00) {
      success = false;
    }
  }
  return success;
}

/**
 * Gets data from MS5607 with specific MS5607Ctrl_t configuration
 * Temperature in C stored in altCtrl->altData.temp
 * Pressure in Atm stored in altCtrl->altData.baro
 */
void MS5607_get_data(MS5607Ctrl_t *altCtrl) {
  // Stores raw pressure and temperature values
  uint8_t presStore[4];
  uint8_t tempStore[4];

  // The commands to request D1 and D2 conversions
  uint8_t convertD1_512 = 0x42;
  uint8_t convertD2_512 = 0x52;

  uint8_t read[4] = {0x00};

  HAL_GPIO_WritePin(altCtrl->spiconfig.port, altCtrl->spiconfig.pin,
                    GPIO_PIN_RESET);
  HAL_SPI_Transmit(altCtrl->spiconfig.hspi, &convertD1_512, 1, SPI_TIMEOUT_MS);
  HAL_Delay(OSR_512);
  HAL_GPIO_WritePin(altCtrl->spiconfig.port, altCtrl->spiconfig.pin,
                    GPIO_PIN_SET);

  HAL_GPIO_WritePin(altCtrl->spiconfig.port, altCtrl->spiconfig.pin,
                    GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(altCtrl->spiconfig.hspi, read, presStore, 4,
                          SPI_TIMEOUT_MS);
  HAL_GPIO_WritePin(altCtrl->spiconfig.port, altCtrl->spiconfig.pin,
                    GPIO_PIN_SET);

  HAL_GPIO_WritePin(altCtrl->spiconfig.port, altCtrl->spiconfig.pin,
                    GPIO_PIN_RESET);
  HAL_SPI_Transmit(altCtrl->spiconfig.hspi, &convertD2_512, 1, SPI_TIMEOUT_MS);
  HAL_Delay(OSR_512);
  HAL_GPIO_WritePin(altCtrl->spiconfig.port, altCtrl->spiconfig.pin,
                    GPIO_PIN_SET);

  HAL_GPIO_WritePin(altCtrl->spiconfig.port, altCtrl->spiconfig.pin,
                    GPIO_PIN_RESET);
  HAL_SPI_TransmitReceive(altCtrl->spiconfig.hspi, read, tempStore, 4,
                          SPI_TIMEOUT_MS);
  HAL_GPIO_WritePin(altCtrl->spiconfig.port, altCtrl->spiconfig.pin,
                    GPIO_PIN_SET);

  uint32_t D1 = (presStore[1] << 16 | presStore[2] << 8 | presStore[3]);
  uint32_t D2 = (tempStore[1] << 16 | tempStore[2] << 8 | tempStore[3]);

  int32_t dT = D2 - ((int32_t)(altCtrl->promVals[4]) << 8);
  volatile int64_t Temp = 2000 + (((int64_t)(altCtrl->promVals[5]) * dT) >> 23);
  volatile int64_t OFF = ((int64_t)(altCtrl->promVals[1]) << 17) +
                         (((int64_t)(altCtrl->promVals[3]) * dT) >> 6);
  volatile int64_t SENS = ((int64_t)(altCtrl->promVals[0]) << 16) +
                          (((int64_t)(altCtrl->promVals[2]) * dT) >> 7);

  // perform higher order corrections
  int64_t T2 = 0, OFF2 = 0, SENS2 = 0;
  if (Temp < 2000) {
    T2 = (dT * dT) >> 31;
    OFF2 = (61 * (Temp - 2000) * (Temp - 2000)) >> 4;
    SENS2 = 2 * (Temp - 2000) * (Temp - 2000);
    if (Temp < -1500) {
      OFF2 += 15 * (Temp + 1500) * (Temp + 1500);
      SENS2 += 8 * (Temp + 1500) * (Temp + 1500);
    }
  }

  Temp -= T2;
  OFF -= OFF2;
  SENS -= SENS2;

  volatile int64_t P = ((((D1 * SENS) >> 21) - OFF) >> 15);

  volatile double Tfinal = Temp / 100.0;
  altCtrl->altData.temp = Tfinal;
  volatile double Pfinal = P / 101325.0;
  altCtrl->altData.baro = Pfinal;
}

#endif
