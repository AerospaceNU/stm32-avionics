/*
 * orientation_estimation.h
 *
 *  Created on: Mar 21, 2022
 *      Author: sam
 */
#include "matrix.h"
#ifndef SYSTEM_FILTERING_ORIENTATION_ESTIMATION_H_
#define SYSTEM_FILTERING_ORIENTATION_ESTIMATION_H_

class OrientationEstimation {
 public:
  double m_dt;
  Matrix<4, 1> q;
  explicit OrientationEstimation(double dt);

  void reset();

  void setDt(double dt);

  void setAccelVector(float accels[]);

  void update(float gyros[]);
};

#endif  // SYSTEM_FILTERING_ORIENTATION_ESTIMATION_H_
