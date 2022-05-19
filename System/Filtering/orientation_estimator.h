/*
 * orientation_estimation.h
 *
 *  Created on: Mar 21, 2022
 *      Author: sam
 */
#include "matrix.h"
#ifndef SYSTEM_FILTERING_ORIENTATION_ESTIMATOR_H_
#define SYSTEM_FILTERING_ORIENTATION_ESTIMATOR_H_

class OrientationEstimator {
 public:
  double m_dt;
  Matrix<4, 1> q;
  explicit OrientationEstimator(double dt);

  void reset();

  void setDt(double dt);

  void setAccelVector(double accels[]);

  void update(float gyros[]);
};

#endif  // SYSTEM_FILTERING_ORIENTATION_ESTIMATOR_H_
