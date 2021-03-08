/*
 * filters.h
 */

#ifndef FILTERS_H_
#define FILTERS_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "hardware_manager.h"

typedef struct {
	double heading;
	double vtg;
	double pos_x;
	double pos_y;
	double pos_z;
	double vel_x;
	double vel_y;
	double vel_z;
	double acc_x;
	double acc_y;
	double acc_z;
	double jerk_x;
	double jerk_y;
	double jerk_z;
	double qx;
	double qy;
	double qz;
	double qw;
} FilterData_t;

/**
 * @brief Applies filters based on current sensor data
 */
void applyFilterData(SensorData_t* curSensorVals);

/**
 * @brief Returns most recent filtered data
 */
FilterData_t* getFilteredData();

#ifdef __cplusplus
}
#endif

#endif /* FILTERS_H_ */
