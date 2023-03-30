/*
 * imu_icm20600.h
 *
 *  Created on: May 28, 2020
 *      Author: John
 */

#ifndef DEVICES_IMU_ICM20600_H_
#define DEVICES_IMU_ICM20600_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "board_config_common.h"
#include "spi_driver.h"

/** Enumerated value corresponds with ACCEL_FS_SEL in the ACCEL_CONFIG
 * register. Values listed are the full +/- G range. */
typedef enum {
	ICM20600_ACCEL_RANGE_2G = 0,
	ICM20600_ACCEL_RANGE_4G = 1,
	ICM20600_ACCEL_RANGE_8G = 2,
	ICM20600_ACCEL_RANGE_16G = 3,
} ImuICM20600AccelFullscale_e;

typedef enum {
	ICM20602_GYRO_RANGE_250dps = 0,
	ICM20602_GYRO_RANGE_500dps,
	ICM20602_GYRO_RANGE_1000dps,
	ICM20602_GYRO_RANGE_2000dps,
} ImuICM20600GyroFullscale_e;

typedef struct ICM20600Ctrl_s {
	ImuData_s data;
	SpiCtrl_t spi;

	float accelSensitivity;
	float gyroSensitivity;

	int16_t tRawVal; // raw temp

	double tVal;
} ImuICM20600Ctrl_s;

bool icm20600_init(ImuICM20600Ctrl_s *sensor,
		ImuICM20600AccelFullscale_e accelFullscale,
		ImuICM20600GyroFullscale_e gyroFullscale);
void icm20600_getData(ImuICM20600Ctrl_s *sensor);

#ifdef __cplusplus
}
#endif

#endif  // DEVICES_IMU_ICM20600_H_
