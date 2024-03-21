/*
 * board_config.c
 *
 *  Created on: Feb 26, 2024
 *      Author: iris
 */

#include "board_config_common.h"
#include "main.h"

#ifdef HAL_SPI_MODULE_ENABLED
#include "spi.h"
#endif  // HAL_SPI_MODULE_ENABLED

#if HAS_DEV(BAROMETER_MS5607)
SPI_HandleTypeDef* barometerMs5607Hspi[NUM_BAROMETER_MS5607] = {&hspi1};
GPIO_TypeDef* barometerMs5607CsGpioPort[NUM_BAROMETER_MS5607] = {BARO1_CS_Pin_GPIO_Port};
uint16_t barometerMs5607CsPin[NUM_BAROMETER_MS5607] = {BARO1_CS_Pin_Pin};
#endif  // HAS_DEV(BAROMETER_MS5607)

#if HAS_DEV(FLASH_S25FLX)
SPI_HandleTypeDef* flashS25flxHspi[NUM_FLASH_S25FLX] = {&hspi1};
GPIO_TypeDef* flashS25flxCsGpioPort[NUM_FLASH_S25FLX] = {FLASH_CS_GPIO_Port};
uint16_t flashS25flxCsPin[NUM_FLASH_S25FLX] = {FLASH_CS_Pin};
#endif  // HAS_DEV(FLASH_S25FL)

// I just made this up idk
#if HAS_DEV(IMU_ICM20600)
SPI_HandleTypeDef* imuIcm20600Hspi[NUM_IMU_ICM20600] = {&hspi1};
GPIO_TypeDef* imuIcm20600CsGpioPort[NUM_IMU_ICM20600] = {IMU_CS_GPIO_Port,};
uint16_t imuIcm20600CsPin[NUM_IMU_ICM20600] = {IMU_CS_Pin};
#endif  // HAS_DEV(IMU_ICM20600)

#if HAS_DEV(IMU)
const Orientation_s imuBoardToLocal[NUM_IMU][3] = {
    {{AXIS_Y, 1}, {AXIS_X, -1}, {AXIS_Z, -1}},
    {{AXIS_Y, 1}, {AXIS_X, -1}, {AXIS_Z, -1}},
};
int imuAccelFilterPriority[NUM_IMU] = {2, 2};
#endif  // HAS_DEV(IMU)

const Orientation_s magBoardToLocal[NUM_MAG][3] = {
	    {{AXIS_Y, 1}, {AXIS_X, 1}, {AXIS_Z, -1}},
	    {{AXIS_Y, 1}, {AXIS_X, 1}, {AXIS_Z, -1}}};

const uint32_t kFlashSizeBytes[NUM_FLASH] = {0x4000000};
