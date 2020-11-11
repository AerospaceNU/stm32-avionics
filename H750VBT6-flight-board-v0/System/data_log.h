/*
 * data_log.h
 */

#ifndef DATA_LOG_H_
#define DATA_LOG_H_

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>
#include "ms5607.h"
#include "LSM9DS1.h"
#include "H3LIS331DL.h"

void data_log_init(SPI_HandleTypeDef *hspi, GPIO_TypeDef *csPort, uint16_t csPin);

void data_log_write(LSM9DS1Ctrl_t *imu1_data, LSM9DS1Ctrl_t *imu2_data, H3LIS331DLCtrl_t *high_g_data, MS5607Ctrl_t *baro1_data, MS5607Ctrl_t *baro2_data);

uint32_t data_log_read(uint32_t flightNum, uint32_t maxBytes, uint8_t *pdata);

#ifdef __cplusplus
}
#endif


#endif /* DATA_LOG_H_ */
