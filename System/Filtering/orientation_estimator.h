/*
 * orientation_estimator.h
 *
 *  Created on: Mar 21, 2022
 *      Author: sam
 */
#include "matrix.h"
#ifndef SYSTEM_FILTERING_ORIENTATION_ESTIMATOR_H_
#define SYSTEM_FILTERING_ORIENTATION_ESTIMATOR_H_

/* The orientation estimator uses accelerometers, gyroscopes, and the
 * magnetometer (in the future) to estimate rocket orientation.
 * In preflight, we can use the gravity vector from accelerometers to determine
 * the initial orientation of the rocket. This is done in `setAccelVector`.
 * During flight, we can't directly use acceleration to get orientation because
 * of the thrust of the rocket. As such, we integrate
 * angular rates from the gyroscope to update how we think the rocket is
 * oriented in 'update'.
 *
 * A calibrated magnetometer would allow us to determine earth referenced
 * heading and would provide additional information on orientation.
 */

class OrientationEstimator {
 public:
  double m_dt;
  Matrix<4, 1> q;
  /**
   * @brief Constructs the OrientationEstimator object with a given dt
   * @param dt: Interval between subsequent angular velocity
   * measurements, in seconds
   */
  explicit OrientationEstimator(double dt);

  /**
   * @brief Destructor for OrientationEstimator
   */
  ~OrientationEstimator();

  /**
   * @brief Resets the orientation estimate
   */
  void reset();

  /**
   * @brief Sets the dt for the OrientationEstimator
   * @param dt: Interval between subsequent angular velocity measurements, in
   * seconds
   */
  void setDt(double dt);

  /**
   * @brief Sets the estimated orientation based on x, y, and z accelerations
   * from gravity
   * Uses formulas from
   * https://github.com/Mayitzin/ahrs/blob/87d27880fd903be3762c68000cc5dd41c720200d/ahrs/common/orientation.py#L923
   * @param acc_x: x acceleration from gravity
   * @param acc_y: y acceleration from gravity
   * @param acc_z: z acceleration from gravity
   */

  void setAccelVector(float acc_x, float acc_y, float acc_z);

  /**
   * @brief Updates the estimated orientation from given angular rates
   * Uses formulas from
   * https://github.com/Mayitzin/ahrs/blob/87f9210cfcf6c545d86ae8588a93f012020164ee/ahrs/filters/angular.py#L374
   * @param gyr_x: angular velocity around x axis
   * @param gyr_y: angular velocity around y axis
   * @param gyr_z: angular velocity around z axis
   */
  void update(float gyr_x, float gyr_y, float gyr_z);
};

#endif  // SYSTEM_FILTERING_ORIENTATION_ESTIMATOR_H_
