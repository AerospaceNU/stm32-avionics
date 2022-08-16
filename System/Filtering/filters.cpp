/*
 * filters.c
 */

#include "filters.h"

#include <math.h>
#include <string.h>

#include "altitude_kalman.h"
#include "board_config_common.h"
#include "circular_buffer.h"
#include "cli.h"
#include "hardware_manager.h"
#include "orientation_estimator.h"

#define R_DRY_AIR 287.0474909  // J/K/kg
#define G_ACCEL_EARTH 9.80665  // m/s**2
#define BARO_MAX_SPEED 248     // m/s over which to not call correct()

#define ACCEL_FS_THRESHOLD \
  0.9  // How close IMU accel should be to fullscale before ignoring values
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

static double filterAccelOneAxis(double* accelReadings, double* imuReadings,
                                 const SensorProperties_t* sensorProperties) {
  int numAccelsValid = 0;
  double accelSum = 0;
  int highestValidPriority = 0;

  // Only pull data if accel below fullscale and sensor is working
#if HAS_DEV(ACCEL)
  for (int i = 0; i < NUM_ACCEL; i++) {
    if (hardwareStatusAccel[i] &&
        fabs(accelReadings[i]) <
            ACCEL_FS_THRESHOLD * sensorProperties->accelFs[i]) {
      if (accelFilterPriority[i] > highestValidPriority) {
        numAccelsValid = 1;
        accelSum = accelReadings[i];
        highestValidPriority = accelFilterPriority[i];
      } else if (accelFilterPriority[i] == highestValidPriority) {
        numAccelsValid++;
        accelSum += accelReadings[i];
      }
    }
  }
#endif  // HAS_DEV(ACCEL)

#if HAS_DEV(IMU)
  for (int i = 0; i < NUM_IMU; i++) {
    if (hardwareStatusImu[i] &&
        fabs(imuReadings[i]) <
            ACCEL_FS_THRESHOLD * sensorProperties->imuAccelFs[i]) {
      if (imuAccelFilterPriority[i] > highestValidPriority) {
        numAccelsValid = 1;
        accelSum = imuReadings[i];
        highestValidPriority = imuAccelFilterPriority[i];
      } else if (imuAccelFilterPriority[i] == highestValidPriority) {
        numAccelsValid++;
        accelSum += imuReadings[i];
      }
    }
  }
#endif  // HAS_DEV(IMU)

  return numAccelsValid == 0 ? 0.0 : accelSum / numAccelsValid;
}

static double filterGyroOneAxis(double* imuReadings) {
  int numGyrosValid = 0;
  double gyroSum = 0;

#if HAS_DEV(IMU)
  for (int i = 0; i < NUM_IMU; i++) {
    if (hardwareStatusImu[i]) {
      numGyrosValid++;
      gyroSum += imuReadings[i];
    }
  }
#endif  // HAS_DEV(IMU)

  return numGyrosValid == 0 ? 0.0 : gyroSum / numGyrosValid;
}

static double getSensorAxis(const Axis_t boardAxis,
                            const Orientation_t* sensorOrientation,
                            double* sensorVals) {
  Orientation_t sensorOrient = sensorOrientation[boardAxis];
  double multiplier = sensorOrient.direction;
  // The gravity ref tells us whether or not we need to rotate our coordinate
  // system around the x axis to flip the gravity (z) direction. We're assuming
  // that the z axis is always the correct axis, just possibly reversed. To flip
  // around the y axis, we need to negate x and z, but not y
  if (boardAxis != AXIS_Y) {
    multiplier *= gravityRef;
  }

  return multiplier * sensorVals[sensorOrient.axis];
}

static void filterAccels(SensorData_t* curSensorVals,
                         SensorProperties_t* sensorProperties) {
  // Get acceleration readings along raw axes
#if HAS_DEV(ACCEL)
  double accelData[NUM_ACCEL][3] = {0};
  for (int i = 0; i < NUM_ACCEL; i++) {
    memcpy(accelData[i], &curSensorVals->accelData[i].realMps2,
           3 * sizeof(double));
  }
#endif  // HAS_DEV(ACCEL)
#if HAS_DEV(IMU)
  double imuAccelData[NUM_IMU][3] = {0};
  for (int i = 0; i < NUM_IMU; i++) {
    memcpy(imuAccelData[i], &curSensorVals->imuData[i].accelRealMps2,
           3 * sizeof(double));
  }
#endif  // HAS_DEV(IMU)

  // Get acceleration readings along real-world axes
#if HAS_DEV(ACCEL)
  double accelReadings[3][NUM_ACCEL];
  for (int i = 0; i < NUM_ACCEL; i++) {
    for (int axis = 0; axis < 3; axis++) {
      accelReadings[axis][i] =
          getSensorAxis((Axis_t)axis, accelBoardToLocal[i], accelData[i]);
    }
  }
#else
  double* accelReadings[3] = {NULL};
#endif  // HAS_DEV(ACCEL)
#if HAS_DEV(IMU)
  double imuReadings[3][NUM_IMU];
  for (int i = 0; i < NUM_IMU; i++) {
    for (int axis = 0; axis < 3; axis++) {
      imuReadings[axis][i] =
          getSensorAxis((Axis_t)axis, imuBoardToLocal[i], imuAccelData[i]);
    }
  }
#else
  double* imuReadings[3] = {NULL};
#endif  // HAS_DEV(IMU)

  // Filter out acceleration data by fullscale range and sensor status
  filterData.rocket_acc_x = filterAccelOneAxis(
      accelReadings[AXIS_X], imuReadings[AXIS_X], sensorProperties);
  filterData.rocket_acc_y = filterAccelOneAxis(
      accelReadings[AXIS_Y], imuReadings[AXIS_Y], sensorProperties);
  filterData.rocket_acc_z = filterAccelOneAxis(
      accelReadings[AXIS_Z], imuReadings[AXIS_Z], sensorProperties);
}

double filterGetAveragePressure(SensorData_t* curSensorVals) {
  double presAvg = 0;
#if HAS_DEV(BAROMETER)
  for (int i = 0; i < NUM_BAROMETER; i++) {
    presAvg += curSensorVals->barometerData[i].pressureAtm;
  }
  presAvg /= NUM_BAROMETER;
#endif  // HAS_DEV(BAROMETER)
  return presAvg;
}

static void filterGyros(SensorData_t* curSensorVals) {
#if HAS_DEV(IMU)
  // Get gyro readings along raw axes
  double imuGyroData[NUM_IMU][3];
  for (int i = 0; i < NUM_IMU; i++) {
    memcpy(imuGyroData[i], &curSensorVals->imuData[i].angVelRealRadps,
           sizeof(double) * 3);
  }

  // Get gyro readings along real-world axes
  double imuReadings[3][NUM_IMU];
  for (int i = 0; i < NUM_IMU; i++) {
    for (int axis = 0; axis < 3; axis++) {
      imuReadings[axis][i] =
          getSensorAxis((Axis_t)axis, imuBoardToLocal[i], imuGyroData[i]);
    }
  }
#else
  double* imuReadings[3] = {NULL};
#endif  // HAS_DEV(IMU)

  filterData.rocket_ang_vel_x =
      filterGyroOneAxis(imuReadings[AXIS_X]) - gyroXOffset;
  filterData.rocket_ang_vel_y =
      filterGyroOneAxis(imuReadings[AXIS_Y]) - gyroYOffset;
  filterData.rocket_ang_vel_z =
      filterGyroOneAxis(imuReadings[AXIS_Z]) - gyroZOffset;

  orientationEstimator.update(filterData.rocket_ang_vel_x,
                              filterData.rocket_ang_vel_y,
                              filterData.rocket_ang_vel_z);

  //  Copy quaternion to filter data
  filterData.qw = orientationEstimator.q(0, 0);
  filterData.qx = orientationEstimator.q(1, 0);
  filterData.qy = orientationEstimator.q(2, 0);
  filterData.qz = orientationEstimator.q(3, 0);
}

void updateGyroOffsetOneAxis(CircularBuffer_t* refBuffer, const float& newValue,
                             float* offset) {
  uint8_t i;
  float gyroSum = 0;
  static float referenceBuffer[kGyroRefCount];
  if (cbFull(refBuffer)) {
    cbDequeue(refBuffer, 1);
  }
  cbEnqueue(refBuffer, &newValue);
  cbPeek(refBuffer, referenceBuffer, nullptr);

  for (i = 0; i < cbCount(refBuffer); ++i) {
    gyroSum += referenceBuffer[i];
  }

  *offset = gyroSum / (float)cbCount(refBuffer);
}

void filterAddGyroRef() {
  static float noOffset;
  noOffset = filterData.rocket_ang_vel_x + gyroXOffset;
  updateGyroOffsetOneAxis(&gyroXRefBuffer, noOffset, &gyroXOffset);
  noOffset = filterData.rocket_ang_vel_y + gyroYOffset;
  updateGyroOffsetOneAxis(&gyroYRefBuffer, noOffset, &gyroYOffset);
  noOffset = filterData.rocket_ang_vel_z + gyroZOffset;
  updateGyroOffsetOneAxis(&gyroZRefBuffer, noOffset, &gyroZOffset);
}

static void filterPositionZ(SensorData_t* curSensorVals, bool hasPassedApogee) {
  // For now, just convert pressure to altitude using the following formula
  // without any sort of real filtering
  // https://en.wikipedia.org/wiki/Barometric_formula
#if HAS_DEV(BAROMETER)
  double lapseRate = -0.0065;  // valid below 11000m, use lookup table for
                               // lapse rate when altitude is higher
  double tempRef = cliGetConfigs()->groundTemperatureC + 273.15;  // C to K
  double elevRef = cliGetConfigs()->groundElevationM;

  double presAvg = filterGetAveragePressure(curSensorVals);
  double baroAlt =
      fabs(presAvg) < 0.001
          ? 0
          : (tempRef / lapseRate) *
                    (1 - pow(presAvg / presRef,
                             R_DRY_AIR * lapseRate / G_ACCEL_EARTH)) +
                elevRef;
#endif  // HAS_DEV(BAROMETER)

  // Kalman filtering, assuming Z is always up
  // TODO rotate Z by orientation

  // If we're descending, we can't trust the acceleration from our
  // accelerometer
  // -- the rocket's flapping around on a parachute! Otherwise, our "global
  // acceleration" is the measured acceleration minus 9.81 (since earth pulls
  // us down). Under parachute it's about zero, since we're about-ish in
  // equilibrium
  double accz = hasPassedApogee ? 0 : filterData.world_acc_z - 9.81;

  // TODO check what order these "should" run in. Worst case we're off by one
  // iteration We don't update accelerations till after this, so the z
  // acceleration should still be from the last timestep
  kalman.Predict(accz);

#if HAS_DEV(BAROMETER)
  // Only correct if below max speed (above, baro readings untrustworthy)
  if (fabs(kalman.GetXhat().estimatedVelocity) < BARO_MAX_SPEED) {
    kalman.Correct(baroAlt, kalman.DEFAULT_KALMAN_GAIN);
  }
#endif  // HAS_DEV(BAROMETER)

  auto kalmanOutput = kalman.GetXhat();
  filterData.pos_z = kalmanOutput.estimatedAltitude;
  filterData.rocket_vel_x = kalmanOutput.estimatedVelocity;
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
  cbEnqueue(&runningGravityRefBuffer, &(filterData.rocket_acc_x));

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
  orientationEstimator.setAccelVector(filterData.rocket_acc_x,
                                      filterData.rocket_acc_y,
                                      filterData.rocket_acc_z);

  // If the sum is negative, we should flip, switch gravity ref and flush
  // buffer
  if (accelSum < 0) {
    gravityRef *= -1;
    cbFlush(&runningGravityRefBuffer);
  }
}

void filterAddPressureRef(SensorData_t* curSensorVals) {
  // Average current pressures
  double currentPres = filterGetAveragePressure(curSensorVals);

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

static void filterSetWorldReference() {
  /*
   * Data comes in through sensors relative to the board. Other filter functions
   * convert sensor measurements into the board reference frame. We then also
   * need to convert this into the world reference frame.
   */

  // How this works is going to change significantly, so this is a very basic
  // transform
  filterData.world_acc_x = filterData.rocket_acc_z;
  filterData.world_acc_y = filterData.rocket_acc_y;
  filterData.world_acc_z = filterData.rocket_acc_x;
  filterData.world_vel_x = filterData.rocket_vel_z;
  filterData.world_vel_y = filterData.rocket_vel_y;
  filterData.world_vel_z = filterData.rocket_vel_x;
}

void filterApplyData(SensorData_t* curSensorVals,
                     SensorProperties_t* sensorProperties,
                     bool hasPassedApogee) {
  // Filter z pos first so we still have the old accelerations
  // This lets us project our state estimate forward from the last
  // timestep to the current one
  filterPositionZ(curSensorVals, hasPassedApogee);

  filterAccels(curSensorVals, sensorProperties);

  filterSetWorldReference();

  filterGyros(curSensorVals);
}

FilterData_t* filterGetData() { return &filterData; }
