/*
 * sensor_types.h
 *
 *  Created on: Mar 7, 2022
 *      Author: matth
 */

#ifndef DEVICES_SENSOR_TYPES_H_
#define DEVICES_SENSOR_TYPES_H_

// Can't be an enum as the preprocessor runs before compiler
#define IMU_ICM20948 1
#define IMU_LSM9DS1 2

#define BARO_MS5607_SPI 1
#define BARO_MS5607_I2C 2

#endif  // DEVICES_SENSOR_TYPES_H_
