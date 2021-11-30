/*
 * filters.c
 */

#include "filters.h"

#include <cmath>

#include "state_cli_config.h"

#define R_DRY_AIR 287.0474909 // J/K/kg
#define G_ACCEL_EARTH 9.80665 // m/s**2

static FilterData_t filterData;
static double presRef = 1013; // mbar

static void filterAccels(SensorData_t* curSensorVals) {
	// For now, just worry about z accel. Take accelerometer value, converting , and averaging
	filterData.acc_z = abs((curSensorVals->imu1_accel_y + curSensorVals->imu2_accel_y) / 2);
}

static void filterPositionZ(SensorData_t* curSensorVals) {
	// For now, just convert pressure to altitude using the following formula without any sort of real filtering
	// https://en.wikipedia.org/wiki/Barometric_formula
	double lapseRate = -0.0065; // valid below 11000m, use lookup table for lapse rate when altitude is higher
	double tempRef = CliConfigState::getConfigs()->groundTemperatureC + 273.15; // C to K
	double elevRef = CliConfigState::getConfigs()->groundElevationM;
	double presAvg = (curSensorVals->baro1_pres + curSensorVals->baro2_pres) / 2;
	filterData.pos_z = (tempRef / lapseRate) * (1 - pow(presAvg / presRef, R_DRY_AIR * lapseRate / G_ACCEL_EARTH)) + elevRef;
}

void filterSetPressureRef(double pres) {
	presRef = pres;
}

void filterApplyData(SensorData_t* curSensorVals) {
	filterPositionZ(curSensorVals);
	filterAccels(curSensorVals);
}

FilterData_t* filterGetData() {
	return &filterData;
}

