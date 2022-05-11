/*
 * filters.c
 */

#include "filters.h"

#include <math.h>
#include <string.h>

#include "altitude_kalman.h"
#include "board_config.h"
#include "cli.h"
#include "hardware_manager.h"

#define R_DRY_AIR 287.0474909  // J/K/kg
#define G_ACCEL_EARTH 9.80665  // m/s**2

#define ACCEL_SWITCH_MULTIPLE \
  0.9  // How close IMU accel should be to fullscale before switching to
       // higher-range accelerometer
#define kPrevPresMedianCount 10
#define kPrevPresCount 10
#define kGravityRefCount 10

static double runningPresMedians[kPrevPresMedianCount + 1];
static CircularBuffer_t runningPresMediansBuffer;
static uint8_t runningPresMedianCount;
static double runningPres[kPrevPresCount + 1];
static CircularBuffer_t runningPresBuffer;
static uint8_t runningPresCount;

static double gravityRefBuffer[kGravityRefCount];
static double runningGravityRef[kGravityRefCount + 1];
static CircularBuffer_t runningGravityRefBuffer;

static FilterData_t filterData;
static double presRef = 1.0;   // atm
static int8_t gravityRef = 1;  // direction of gravity

static double medianArray[kPrevPresMedianCount + kPrevPresCount];

// Sane default
static AltitudeKalman kalman{0.015};

void filterInit(double dt) {
  kalman.Reset();
  kalman.SetDt(dt);
  // Initialize circular buffers for running medians/running pressure values
  cbInit(&runningPresMediansBuffer, runningPresMedians,
         kPrevPresMedianCount + 1, sizeof(double));
  cbInit(&runningPresBuffer, runningPres, kPrevPresCount + 1, sizeof(double));
  cbInit(&runningGravityRefBuffer, runningGravityRef, kGravityRefCount + 1,
         sizeof(double));
  runningPresMedianCount = 0;
  runningPresCount = 0;
}

static double filterAccelOneAxis(const double imu1reading,
                                 const double imu2reading,
                                 const double highGreading,
                                 SensorProperties_t* sensorProperties,
                                 bool imu1sampling, bool imu2sampling,
                                 bool highGsampling) {
  int numAccelsValid = 0;
  double accelSum = 0;

  // Only pull from IMUs if accel below fullscale and if they're working
  if (fabs(imu1reading) <
          ACCEL_SWITCH_MULTIPLE * sensorProperties->imu1_accel_fs &&
      imu1sampling) {
    numAccelsValid++;
    accelSum += imu1reading;
  }
  if (fabs(imu2reading) <
          ACCEL_SWITCH_MULTIPLE * sensorProperties->imu2_accel_fs &&
      imu2sampling) {
    numAccelsValid++;
    accelSum += imu2reading;
  }

  // If our IMUs are fullscaled, pull from high-G
  if (numAccelsValid == 0 && highGsampling) {
    numAccelsValid++;
    accelSum += highGreading;
  }
  return accelSum / (double)numAccelsValid;
}

static double getSensorAccelAxis(const Axis_t boardAxis,
                                 const Orientation_t* sensorOrientation,
                                 double* sensorAccels) {
  Orientation_t sensorOrient = sensorOrientation[boardAxis];
  double multiplier = sensorOrient.direction;
  // The gravity ref tells us whether or not we need to rotate our coordinate
  // system around the x axis to flip the gravity (z) direction. We're assuming
  // that the z axis is always the correct axis, just possibly reversed. To flip
  // around the x axis, we need to negate y and z, but not x
  if (boardAxis != AXIS_X) {
    multiplier *= gravityRef;
  }

  return multiplier * sensorAccels[sensorOrient.axis];
}

static void filterAccels(SensorData_t* curSensorVals,
                         SensorProperties_t* sensorProperties) {
  bool* status = HM_GetHardwareStatus();

  double imu1_data[3] = {0};
  memcpy(imu1_data, &curSensorVals->imu1_accel_x, 3 * sizeof(double));
  double imu2_data[3] = {0};
  memcpy(imu2_data, &curSensorVals->imu2_accel_x, 3 * sizeof(double));
  double high_g_data[3] = {0};
  memcpy(high_g_data, &curSensorVals->high_g_accel_x, 3 * sizeof(double));

  filterData.acc_x = (filterAccelOneAxis(
      getSensorAccelAxis(AXIS_X, IMU1_ACCEL_BOARD_TO_LOCAL, imu1_data),
      getSensorAccelAxis(AXIS_X, IMU2_ACCEL_BOARD_TO_LOCAL, imu2_data),
      getSensorAccelAxis(AXIS_X, HIGH_G_ACCEL_BOARD_TO_LOCAL, high_g_data),
      sensorProperties, status[IMU1], status[IMU2],
      status[HIGH_G_ACCELEROMETER]));

  filterData.acc_y = (filterAccelOneAxis(
      getSensorAccelAxis(AXIS_Y, IMU1_ACCEL_BOARD_TO_LOCAL, imu1_data),
      getSensorAccelAxis(AXIS_Y, IMU2_ACCEL_BOARD_TO_LOCAL, imu2_data),
      getSensorAccelAxis(AXIS_Y, HIGH_G_ACCEL_BOARD_TO_LOCAL, high_g_data),
      sensorProperties, status[IMU1], status[IMU2],
      status[HIGH_G_ACCELEROMETER]));

  filterData.acc_z = (filterAccelOneAxis(
      getSensorAccelAxis(AXIS_Z, IMU1_ACCEL_BOARD_TO_LOCAL, imu1_data),
      getSensorAccelAxis(AXIS_Z, IMU2_ACCEL_BOARD_TO_LOCAL, imu2_data),
      getSensorAccelAxis(AXIS_Z, HIGH_G_ACCEL_BOARD_TO_LOCAL, high_g_data),
      sensorProperties, status[IMU1], status[IMU2],
      status[HIGH_G_ACCELEROMETER]));
}

static void filterPositionZ(SensorData_t* curSensorVals, bool hasPassedApogee) {
  // For now, just convert pressure to altitude using the following formula
  // without any sort of real filtering
  // https://en.wikipedia.org/wiki/Barometric_formula
  double lapseRate = -0.0065;  // valid below 11000m, use lookup table for lapse
                               // rate when altitude is higher
  double tempRef = cliGetConfigs()->groundTemperatureC + 273.15;  // C to K
  double elevRef = cliGetConfigs()->groundElevationM;
  double presAvg = (curSensorVals->baro1_pres + curSensorVals->baro2_pres) / 2;
  double baroAlt =
      fabs(presAvg) < 0.001
          ? 0
          : (tempRef / lapseRate) *
                    (1 - pow(presAvg / presRef,
                             R_DRY_AIR * lapseRate / G_ACCEL_EARTH)) +
                elevRef;

  // Kalman filtering, assuming Z is always up
  // TODO rotate Z by orientation

  // If we're descending, we can't trust the acceleration from our accelerometer
  // -- the rocket's flapping around on a parachute! Otherwise, our "global
  // acceleration" is the measured acceleration minus 9.81 (since earth pulls us
  // down). Under parachute it's about zero, since we're about-ish in
  // equilibrium
  double accz = hasPassedApogee ? 0 : filterData.acc_z - 9.81;

  // TODO check what order these "should" run in. Worst case we're off by one
  // iteration We don't update accelerations till after this, so the z
  // acceleration should still be from the last timestep
  kalman.Predict(accz);
  kalman.Correct(baroAlt, DEFAULT_KALMAN_GAIN);

  auto kalmanOutput = kalman.GetXhat();
  filterData.pos_z = kalmanOutput.estimatedAltitude;
  filterData.vel_z = kalmanOutput.estimatedVelocity;
}

static double median(double* input, uint8_t count) {
  // Insertion sort
  int8_t i, j;
  double tmp;
  for (i = 1; i < count; ++i) {
    tmp = input[i];
    j = i - 1;
    while (j >= 0 && tmp <= input[j]) {
      input[j + 1] = input[j];
      --j;
    }
    input[j + 1] = tmp;
  }

  if (count % 2 == 0) {
    return (input[count / 2 - 1] + input[count / 2]) / 2.0;
  } else {
    return input[count / 2];
  }
}

void filterAddGravityRef() {
  if (cbFull(&runningGravityRefBuffer)) {
    cbDequeue(&runningGravityRefBuffer, 1);
  }
  cbEnqueue(&runningGravityRefBuffer, &(filterData.acc_z));

  cbPeek(&runningGravityRefBuffer, gravityRefBuffer, nullptr);

  uint8_t gravCount = cbCount(&runningGravityRefBuffer);

  double accelSum = 0;
  for (uint8_t i = 0; i < gravCount; ++i) {
    accelSum += gravityRefBuffer[i];
  }

  // Check that we are mostly in the direction of gravity in some direction.
  // If the sum of our accelerations averages out to less than half G per
  // reaing, don't flip gravity
  if (fabs(accelSum) < 0.5 * G_ACCEL_EARTH * gravCount) return;

  // If the sum is negative, we should flip, switch gravity ref and flush
  // buffer
  if (accelSum < 0) {
    gravityRef *= -1;
    cbFlush(&runningGravityRefBuffer);
  }
}

void filterAddPressureRef(double currentPres) {
  // For the first 10 seconds (before we have any current medians
  // just set the current pressure ref so we don't depend on
  // a single initial value
  if (runningPresMedianCount == 0) {
    presRef = currentPres;
  }
  // Make room for new value, discarding oldest pressure stored if full
  if (cbFull(&runningPresBuffer)) {
    cbDequeue(&runningPresBuffer, 1);
  }

  // Add current pressure
  cbEnqueue(&runningPresBuffer, &currentPres);

  ++runningPresCount;

  // Add median to the running medians every n values
  if (runningPresCount == kPrevPresCount) {
    runningPresCount = 0;
    if (runningPresMedianCount < kPrevPresMedianCount) ++runningPresMedianCount;
    // Make room for new value, discarding oldest median stored if full
    if (cbFull(&runningPresMediansBuffer)) {
      cbDequeue(&runningPresMediansBuffer, 1);
    }

    // Add median of most recent values
    size_t numElements = kPrevPresCount;
    cbPeek(&runningPresBuffer, medianArray, &numElements);
    double currentMedian = median(medianArray, kPrevPresCount);
    cbEnqueue(&runningPresMediansBuffer, &currentMedian);

    // Only set pressure ref if we have enough values recorded
    if (runningPresMedianCount == kPrevPresMedianCount) {
      // Now, find median of the last 100 seconds of data and set that to be
      // current ref
      size_t numMedElements = kPrevPresMedianCount;
      cbPeek(&runningPresMediansBuffer, medianArray, &numMedElements);
      presRef = median(medianArray, kPrevPresMedianCount);
    } else {
      // Otherwise (for the first 100 seconds) set current pressure ref to the
      // median of the last 10 seconds so that we have at least a somewhat
      // trustworthy reference
      presRef = runningPresMedians[0];
    }
  }
}

void filterSetPressureRef(double pres) { presRef = pres; }

double filterGetPressureRef() { return presRef; }

void filterApplyData(SensorData_t* curSensorVals,
                     SensorProperties_t* sensorProperties,
                     bool hasPassedApogee) {
  // Filter z pos first so we still have the old accelerations
  // This lets us project our state estimate forward from the last
  // timestep to the current one
  filterPositionZ(curSensorVals, hasPassedApogee);

  filterAccels(curSensorVals, sensorProperties);
}

FilterData_t* filterGetData() { return &filterData; }
