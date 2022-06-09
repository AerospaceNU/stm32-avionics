/*
 * orientation_estimator.cpp
 *
 *  Created on: Mar 21, 2022
 *      Author: sam
 */
#include <math.h>
#include <orientation_estimator.h>

#include "matrix.h"

OrientationEstimator::OrientationEstimator(double dt) : m_dt(dt) {}

void OrientationEstimator::reset() { this->q = Matrix<4, 1>({1.0, 0, 0, 0}); }

void OrientationEstimator::setDt(double dt) { this->m_dt = dt; }

void OrientationEstimator::setAccelVector(float acc_x, float acc_y,
                                          float acc_z) {
  // From
  // https://github.com/Mayitzin/ahrs/blob/87d27880fd903be3762c68000cc5dd41c720200d/ahrs/common/orientation.py#L923
  Matrix<3, 1> a({acc_x, acc_y, acc_z});
  float a_norm = a.norm();
  if (!(a_norm > 9.0 && a_norm < 11.0)) return;
  float ax = acc_x / a_norm;
  float ay = acc_y / a_norm;
  float az = acc_z / a_norm;
  float ex = atan2(ay, az);
  float ey = atan2(-ax, sqrt(pow(ay, 2) + pow(az, 2)));
  float cx2 = cos(ex / 2.0);
  float sx2 = sin(ex / 2.0);
  float cy2 = cos(ey / 2.0);
  float sy2 = sin(ey / 2.0);
  this->q = Matrix<4, 1>({cx2 * cy2, sx2 * cy2, cx2 * sy2, -sx2 * sy2});
  this->q = q * (1.0 / q.norm());
}

void OrientationEstimator::update(float gyr[]) {
  // From
  // https://github.com/Mayitzin/ahrs/blob/87f9210cfcf6c545d86ae8588a93f012020164ee/ahrs/filters/angular.py#L374
  Matrix<4, 4> omega({0.0, -gyr[0], -gyr[1], -gyr[2], gyr[0], 0.0, gyr[2],
                      -gyr[1], gyr[1], -gyr[2], 0.0, gyr[0], gyr[2], gyr[1],
                      -gyr[0], 0.0});
  Matrix<3, 1> gyro({gyr[0], gyr[1], gyr[2]});

  volatile float w = gyro.norm();
  Matrix<4, 4> ident = (Matrix<4, 4>::identity() * cos(w * m_dt / 2.0));
  float omgmult = (1.0 / w) * sin(w * m_dt / 2.0);
  Matrix<4, 4> A = ident + omega * omgmult;
  this->q = A * q;
  this->q = q * (1.0 / q.norm());
}
