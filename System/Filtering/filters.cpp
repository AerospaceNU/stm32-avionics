/*
 * filters.c
 */

#include "filters.h"

#include <cmath>

#include "altitude_kalman.h"
#include "cli.h"

#define R_DRY_AIR 287.0474909 // J/K/kg
#define G_ACCEL_EARTH 9.80665 // m/s**2

#define ACCEL_SWITCH_MULTIPLE 0.9 // How close IMU accel should be to fullscale before switching to higher-range accelerometer

static FilterData_t filterData;
static double presRef = 1.0; // atm

// Sane default
static AltitudeKalman kalman {0.015};

void filterInit(double dt) {
	kalman.SetDt(dt);
}

static void filterAccels(SensorData_t* curSensorVals, SensorProperties_t* sensorProperties) {

	// For now, just worry about z accel
	int numAccelsValid = 0;
	double accelSum = 0;
	if (fabs(curSensorVals->imu1_accel_y) < ACCEL_SWITCH_MULTIPLE * sensorProperties->imu1_accel_fs) {
		numAccelsValid++;
		accelSum += -curSensorVals->imu1_accel_y;
	}
	if (fabs(curSensorVals->imu2_accel_y) < ACCEL_SWITCH_MULTIPLE * sensorProperties->imu2_accel_fs) {
		numAccelsValid++;
		accelSum += -curSensorVals->imu2_accel_y;
	}
	if (numAccelsValid == 0) {
		numAccelsValid++;
		accelSum += curSensorVals->high_g_accel_x;
	}
	filterData.acc_z = abs(accelSum / numAccelsValid);
}

static void filterPositionZ(SensorData_t* curSensorVals, bool hasPassedApogee) {
	// For now, just convert pressure to altitude using the following formula without any sort of real filtering
	// https://en.wikipedia.org/wiki/Barometric_formula
	double lapseRate = -0.0065; // valid below 11000m, use lookup table for lapse rate when altitude is higher
	double tempRef = cliGetConfigs()->groundTemperatureC + 273.15; // C to K
	double elevRef = cliGetConfigs()->groundElevationM;
	double presAvg = (curSensorVals->baro1_pres + curSensorVals->baro2_pres) / 2;
	double baroAlt = fabs(presAvg) < 0.001 ? 0 : (tempRef / lapseRate) * (1 - pow(presAvg / presRef, R_DRY_AIR * lapseRate / G_ACCEL_EARTH)) + elevRef;

	// Kalman filtering, assuming Z is always up
	// TODO rotate Z by orientation

	// If we're descending, we can't trust the acceleration from our accelerometer -- the rocket's
	// flapping around on a parachute! Otherwise, our "global acceleration" is the measured acceleration
	// minus 9.81 (since earth pulls us down). Under parachute it's about zero, since we're about-ish
	// in equilibrium
	double accz = hasPassedApogee ? 0 : filterData.acc_z - 9.81;

	// TODO check what order these "should" run in. Worst case we're off by one iteration
	// We don't update accelerations till after this, so the z acceleration should still be from the last timestep
	kalman.Predict(accz);
	kalman.Correct(baroAlt, DEFAULT_KALMAN_GAIN);

	auto kalmanOutput = kalman.GetXhat();
	filterData.pos_z = kalmanOutput.estimatedAltitude;
	filterData.vel_z = kalmanOutput.estimatedVelocity;


}

void filterSetPressureRef(double pres) {
	presRef = pres;
}

double filterGetPressureRef() {
	return presRef;
}

void filterApplyData(SensorData_t* curSensorVals, SensorProperties_t* sensorProperties, bool hasPassedApogee) {
	// Filter z pos first so we still have the old accelerations
	// This lets us project our state estimate forward from the last
	// timestep to the current one
	filterPositionZ(curSensorVals, hasPassedApogee);

	filterAccels(curSensorVals, sensorProperties);
}

FilterData_t* filterGetData() {
	return &filterData;
}

