/*
 * filters.c
 */

#include "filters.h"

#include <orientation_estimator.h>
#include <string.h>

#include "altitude_kalman.h"
#include "board_config.h"
#include "cli.h"
#include "hardware_manager.h"

#define R_DRY_AIR 287.0474909  // J/K/kg
#define G_ACCEL_EARTH 9.80665  // m/s**2
#define BARO_MAX_SPEED 248     // m/s over which to not call correct()

#define ACCEL_SWITCH_MULTIPLE \
  0.9  // How close IMU accel should be to fullscale before switching to
       // higher-range accelerometer
#define kPrevPresMedianCount 10
#define kPrevPresCount 10
#define kGravityRefCount 10
#define kGyroRefCount 10

static double runningPresMedians[kPrevPresMedianCount + 1];
static CircularBuffer_t runningPresMediansBuffer;
static uint8_t runningPresMedianCount;
static double runningPres[kPrevPresCount + 1];
static CircularBuffer_t runningPresBuffer;
static uint8_t runningPresCount;

static double gravityRefBuffer[kGravityRefCount];
static double runningGravityRef[kGravityRefCount + 1];
static CircularBuffer_t runningGravityRefBuffer;

static float gyroBuff[kGyroRefCount];
static float gyroXRefBack[kGyroRefCount + 1];
static CircularBuffer_t gyroXRefBuffer;
static float gyroXOffset;
static float gyroYRefBack[kGyroRefCount + 1];
static CircularBuffer_t gyroYRefBuffer;
static float gyroYOffset;
static float gyroZRefBack[kGyroRefCount + 1];
static CircularBuffer_t gyroZRefBuffer;
static float gyroZOffset;

static FilterData_t filterData;
static double presRef = 1.0;   // atm
static int8_t gravityRef = 1;  // direction of gravity

static double medianArray[kPrevPresMedianCount + kPrevPresCount];

// Sane default
static AltitudeKalman kalman{0.015};
static OrientationEstimator orientationEstimator(0.015);

void filterInit(double dt) {
  kalman.Reset();
  kalman.SetDt(dt);
  orientationEstimator.reset();
  orientationEstimator.setDt(dt);
  // Initialize circular buffers for running medians/running pressure values
  cbInit(&runningPresMediansBuffer, runningPresMedians,
         kPrevPresMedianCount + 1, sizeof(double));
  cbInit(&runningPresBuffer, runningPres, kPrevPresCount + 1, sizeof(double));
  cbInit(&runningGravityRefBuffer, runningGravityRef, kGravityRefCount + 1,
         sizeof(double));
  cbInit(&gyroXRefBuffer, gyroXRefBack, kGyroRefCount + 1, sizeof(float));
  cbInit(&gyroYRefBuffer, gyroYRefBack, kGyroRefCount + 1, sizeof(float));
  cbInit(&gyroZRefBuffer, gyroZRefBack, kGyroRefCount + 1, sizeof(float));
  gyroXOffset = 0;
  gyroYOffset = 0;
  gyroZOffset = 0;
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

static double filterGyroOneAxis(const double imu1reading,
                                const double imu2reading, bool imu1sampling,
                                bool imu2sampling) {
  int numGyrosValid = 0;
  double gyroSum = 0;

  if (imu1sampling) {
    gyroSum += imu1reading;
    ++numGyrosValid;
  }
  if (imu2sampling) {
    gyroSum += imu2reading;
    ++numGyrosValid;
  }
  return numGyrosValid == 0 ? 0.0 : gyroSum / (double)numGyrosValid;
}

static double getSensorAxis(const Axis_t boardAxis,
                            const Orientation_t* sensorOrientation,
                            double* sensorAccels) {
  Orientation_t sensorOrient = sensorOrientation[boardAxis];
  double multiplier = sensorOrient.direction;
  // The gravity ref tells us whether or not we need to rotate our coordinate
  // system around the x axis to flip the gravity (z) direction. We're assuming
  // that the z axis is always the correct axis, just possibly reversed. To flip
  // around the x axis, we need to negate y and z, but not x
  if (boardAxis != AXIS_Y) {
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
      getSensorAxis(AXIS_X, IMU1_ACCEL_BOARD_TO_LOCAL, imu1_data),
      getSensorAxis(AXIS_X, IMU2_ACCEL_BOARD_TO_LOCAL, imu2_data),
      getSensorAxis(AXIS_X, HIGH_G_ACCEL_BOARD_TO_LOCAL, high_g_data),
      sensorProperties, status[IMU1], status[IMU2],
      status[HIGH_G_ACCELEROMETER]));

  filterData.acc_y = (filterAccelOneAxis(
      getSensorAxis(AXIS_Y, IMU1_ACCEL_BOARD_TO_LOCAL, imu1_data),
      getSensorAxis(AXIS_Y, IMU2_ACCEL_BOARD_TO_LOCAL, imu2_data),
      getSensorAxis(AXIS_Y, HIGH_G_ACCEL_BOARD_TO_LOCAL, high_g_data),
      sensorProperties, status[IMU1], status[IMU2],
      status[HIGH_G_ACCELEROMETER]));

  filterData.acc_z = (filterAccelOneAxis(
      getSensorAxis(AXIS_Z, IMU1_ACCEL_BOARD_TO_LOCAL, imu1_data),
      getSensorAxis(AXIS_Z, IMU2_ACCEL_BOARD_TO_LOCAL, imu2_data),
      getSensorAxis(AXIS_Z, HIGH_G_ACCEL_BOARD_TO_LOCAL, high_g_data),
      sensorProperties, status[IMU1], status[IMU2],
      status[HIGH_G_ACCELEROMETER]));
}

static void filterGyros(SensorData_t* curSensorVals) {
  bool* status = HM_GetHardwareStatus();

  double imu1_data[3] = {0};
  memcpy(imu1_data, &curSensorVals->imu1_gyro_x, 3 * sizeof(double));
  double imu2_data[3] = {0};
  memcpy(imu2_data, &curSensorVals->imu2_gyro_x, 3 * sizeof(double));
  float gyro[3];
  gyro[0] = filterGyroOneAxis(
                getSensorAxis(AXIS_X, IMU1_GYRO_BOARD_TO_LOCAL, imu1_data),
                getSensorAxis(AXIS_X, IMU2_GYRO_BOARD_TO_LOCAL, imu2_data),
                status[IMU1], status[IMU2]) -
            gyroXOffset;

  gyro[1] = filterGyroOneAxis(
                getSensorAxis(AXIS_Y, IMU1_GYRO_BOARD_TO_LOCAL, imu1_data),
                getSensorAxis(AXIS_Y, IMU2_GYRO_BOARD_TO_LOCAL, imu2_data),
                status[IMU1], status[IMU2]) -
            gyroYOffset;

  gyro[2] = filterGyroOneAxis(
                getSensorAxis(AXIS_Z, IMU1_GYRO_BOARD_TO_LOCAL, imu1_data),
                getSensorAxis(AXIS_Z, IMU2_GYRO_BOARD_TO_LOCAL, imu2_data),
                status[IMU1], status[IMU2]) -
            gyroZOffset;

  orientationEstimator.update(gyro);
  // Copy quaternion to filter data

  filterData.gyr_x = gyro[0];
  filterData.gyr_y = gyro[1];
  filterData.gyr_z = gyro[2];

  filterData.qx = orientationEstimator.q(1, 0);
  filterData.qy = orientationEstimator.q(2, 0);
  filterData.qz = orientationEstimator.q(3, 0);
  filterData.qw = orientationEstimator.q(0, 0);
}

void filterAddGyroRef() {
  uint8_t i;
  float gyroSum = 0;
  static float noOffset;

  if (cbFull(&gyroXRefBuffer)) {
    cbDequeue(&gyroXRefBuffer, 1);
  }
  noOffset = filterData.gyr_x + gyroXOffset;
  cbEnqueue(&gyroXRefBuffer, &noOffset);
  cbPeek(&gyroXRefBuffer, gyroBuff, nullptr);

  for (i = 0; i < cbCount(&gyroXRefBuffer); ++i) {
    gyroSum += gyroBuff[i];
  }

  gyroXOffset = gyroSum / (float)cbCount(&gyroXRefBuffer);
  gyroSum = 0;

  if (cbFull(&gyroYRefBuffer)) {
    cbDequeue(&gyroXRefBuffer, 1);
  }

  noOffset = filterData.gyr_y + gyroYOffset;
  cbEnqueue(&gyroYRefBuffer, &noOffset);
  cbPeek(&gyroYRefBuffer, gyroBuff, nullptr);
  for (i = 0; i < cbCount(&gyroYRefBuffer); ++i) {
    gyroSum += gyroBuff[i];
  }

  gyroYOffset = gyroSum / (float)cbCount(&gyroYRefBuffer);
  gyroSum = 0;

  if (cbFull(&gyroZRefBuffer)) {
    cbDequeue(&gyroZRefBuffer, 1);
  }
  noOffset = filterData.gyr_z + gyroZOffset;
  cbEnqueue(&gyroZRefBuffer, &noOffset);
  cbPeek(&gyroZRefBuffer, gyroBuff, nullptr);
  for (i = 0; i < cbCount(&gyroZRefBuffer); ++i) {
    gyroSum += gyroBuff[i];
  }

  gyroZOffset = gyroSum / (float)cbCount(&gyroZRefBuffer);
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
  double accx = hasPassedApogee ? 0 : filterData.acc_x - 9.81;

  // TODO check what order these "should" run in. Worst case we're off by one
  // iteration We don't update accelerations till after this, so the z
  // acceleration should still be from the last timestep
  kalman.Predict(accx);

  // Only correct if below max speed (above, baro readings untrustworthy)
  if (fabs(kalman.GetXhat().estimatedVelocity) < BARO_MAX_SPEED) {
    kalman.Correct(baroAlt, DEFAULT_KALMAN_GAIN);
  }

  auto kalmanOutput = kalman.GetXhat();
  filterData.pos_z = kalmanOutput.estimatedAltitude;
  filterData.vel_x = kalmanOutput.estimatedVelocity;
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
  cbEnqueue(&runningGravityRefBuffer, &(filterData.acc_x));

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

  // We have a semi-realistic gravity vector, and we know we're in
  // preflight. Reset the orientation estimation to this new gravity vector
  orientationEstimator.setAccelVector(&filterData.acc_x);

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

  filterGyros(curSensorVals);
}

FilterData_t* filterGetData() { return &filterData; }
