#include "barometer_ms5607.h"

#if HAS_DEV(BAROMETER_MS5607)

#define SPI_TIMEOUT_MS 50

/**
 * Different OSR resolutions for MS5607 ADCs, mapped to their conversion times
 * in ms
 */
typedef enum {
  OSR_4096 = 10,
  OSR_2048 = 5,
  OSR_1024 = 3,
  OSR_512 = 2,
  OSR_256 = 1
} Ms5607Osr_e;

typedef enum {
  CONVERT_STATE_IDLE = 0,
  CONVERT_STATE_D1,
  CONVERT_STATE_D2,
} ConvertState_e;

/*
 * Initializes MS5607 temperature and pressure sensor
 * with specific MS5607Ctrl_t configuration
 */
bool barometerMs5607_init(BarometerMs5607Ctrl_s *ms5607,
                          SPI_HandleTypeDef *hspi, GPIO_TypeDef *csPort,
                          uint16_t csPin) {
  ms5607->hspi = hspi;
  ms5607->csPort = csPort;
  ms5607->csPin = csPin;
  ms5607->convertState = CONVERT_STATE_IDLE;

  bool success = true;
  uint8_t init = 0x1E;  // Initialization hex command for MS5607

  // Pull CS Low
  HAL_GPIO_WritePin(ms5607->csPort, ms5607->csPin, GPIO_PIN_RESET);

  // Send the initialization command and delay for 3ms
  HAL_SPI_Transmit(ms5607->hspi, &init, 1, SPI_TIMEOUT_MS);
  HAL_Delay(3);

  // Pull CS High
  HAL_GPIO_WritePin(ms5607->csPort, ms5607->csPin, GPIO_PIN_SET);

  // PROM read
  uint8_t rxBuffer[3];
  uint8_t txBuffer[3];
  uint8_t promRead[6] = {0xA2, 0xA4, 0xA6, 0xA8, 0xAA, 0xAC};
  for (int i = 0; i < 6; i++) {
    txBuffer[0] = promRead[i];
    HAL_GPIO_WritePin(ms5607->csPort, ms5607->csPin, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive(ms5607->hspi, txBuffer, rxBuffer, 3,
                            SPI_TIMEOUT_MS);
    HAL_GPIO_WritePin(ms5607->csPort, ms5607->csPin, GPIO_PIN_SET);
    ms5607->promVals[i] = rxBuffer[1] << 8 | rxBuffer[2];
    if (ms5607->promVals[i] == 0xFF || ms5607->promVals[i] == 0x00) {
      success = false;
    }
  }
  return success;
}

bool barometerMs5607_getData(BarometerMs5607Ctrl_s *ms5607) {
  // The commands to request D1 and D2 conversions
  uint8_t convertD1_256 = 0x40;
  uint8_t convertD2_256 = 0x50;

  uint8_t read[4] = {0x00};

  bool complete = false;

  switch (ms5607->convertState) {
    case CONVERT_STATE_IDLE:
      // Request D1 conversion
      HAL_GPIO_WritePin(ms5607->csPort, ms5607->csPin, GPIO_PIN_RESET);
      HAL_SPI_Transmit(ms5607->hspi, &convertD1_256, 1, SPI_TIMEOUT_MS);
      HAL_GPIO_WritePin(ms5607->csPort, ms5607->csPin, GPIO_PIN_SET);

      ms5607->lastConvReqTime = HAL_GetTick();
      ms5607->convertState = CONVERT_STATE_D1;
      break;
    case CONVERT_STATE_D1:
      if (HAL_GetTick() - ms5607->lastConvReqTime < OSR_256) break;

      // Read pressure
      HAL_GPIO_WritePin(ms5607->csPort, ms5607->csPin, GPIO_PIN_RESET);
      HAL_SPI_TransmitReceive(ms5607->hspi, read, ms5607->rawPres, 4,
                              SPI_TIMEOUT_MS);
      HAL_GPIO_WritePin(ms5607->csPort, ms5607->csPin, GPIO_PIN_SET);

      // Request D2 conversion
      HAL_GPIO_WritePin(ms5607->csPort, ms5607->csPin, GPIO_PIN_RESET);
      HAL_SPI_Transmit(ms5607->hspi, &convertD2_256, 1, SPI_TIMEOUT_MS);
      HAL_GPIO_WritePin(ms5607->csPort, ms5607->csPin, GPIO_PIN_SET);

      ms5607->lastConvReqTime = HAL_GetTick();
      ms5607->convertState = CONVERT_STATE_D2;
      break;
    case CONVERT_STATE_D2:
      if (HAL_GetTick() - ms5607->lastConvReqTime < OSR_256) break;

      HAL_GPIO_WritePin(ms5607->csPort, ms5607->csPin, GPIO_PIN_RESET);
      HAL_SPI_TransmitReceive(ms5607->hspi, read, ms5607->rawTemp, 4,
                              SPI_TIMEOUT_MS);
      HAL_GPIO_WritePin(ms5607->csPort, ms5607->csPin, GPIO_PIN_SET);

      uint32_t D1 = (ms5607->rawPres[1] << 16 | ms5607->rawPres[2] << 8 |
                     ms5607->rawPres[3]);
      uint32_t D2 = (ms5607->rawTemp[1] << 16 | ms5607->rawTemp[2] << 8 |
                     ms5607->rawTemp[3]);

      int32_t dT = D2 - ((int32_t)(ms5607->promVals[4]) << 8);
      volatile int64_t Temp =
          2000 + (((int64_t)(ms5607->promVals[5]) * dT) >> 23);
      volatile int64_t OFF = ((int64_t)(ms5607->promVals[1]) << 17) +
                             (((int64_t)(ms5607->promVals[3]) * dT) >> 6);
      volatile int64_t SENS = ((int64_t)(ms5607->promVals[0]) << 16) +
                              (((int64_t)(ms5607->promVals[2]) * dT) >> 7);

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
      ms5607->data.temperatureC = Tfinal;
      volatile double Pfinal = P / 101325.0;
      ms5607->data.pressureAtm = Pfinal;
      ms5607->convertState = CONVERT_STATE_IDLE;
      complete = true;
      break;
    default:
      complete = true;
      break;
  }

  return complete;
}

#endif  // HAS_DEV(BAROMETER_MS5607)
