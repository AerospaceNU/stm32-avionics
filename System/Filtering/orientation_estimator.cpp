/*
 * orientation_estimator.cpp
 *
 *  Created on: Mar 21, 2022
 *      Author: sam
 */
#include <math.h>
#include <orientation_estimator.h>

#include "matrix.h"

#define G_THRESHOLD_MIN 9.0
#define G_THRESHOLD_MAX 11.0

OrientationEstimator::OrientationEstimator(double dt) : m_dt(dt) {}

OrientationEstimator::~OrientationEstimator() {}

void OrientationEstimator::reset() { this->q = Matrix<4, 1>({1.0, 0, 0, 0}); }

void OrientationEstimator::setDt(double dt) { this->m_dt = dt; }

void OrientationEstimator::setAccelVector(float acc_x, float acc_y,
                                          float acc_z) {
  Matrix<3, 1> a({acc_x, acc_y, acc_z});
  float a_norm = a.norm();
  if (!(a_norm > G_THRESHOLD_MIN && a_norm < G_THRESHOLD_MAX)) return;
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
  this->q = q / q.norm();
}

void OrientationEstimator::update(float ang_vel_x, float ang_vel_y,
                                  float ang_vel_z) {
  Matrix<4, 4> omega({0.0, -ang_vel_x, -ang_vel_y, -ang_vel_z, ang_vel_x, 0.0,
                      ang_vel_z, -ang_vel_y, ang_vel_y, -ang_vel_z, 0.0,
                      ang_vel_x, ang_vel_z, ang_vel_y, -ang_vel_x, 0.0});
  Matrix<3, 1> gyro({ang_vel_x, ang_vel_y, ang_vel_z});

  volatile float w = gyro.norm();
  Matrix<4, 4> ident = (Matrix<4, 4>::identity() * cos(w * m_dt / 2.0));
  float omgmult = sin(w * m_dt / 2.0) / w;
  Matrix<4, 4> A = ident + omega * omgmult;
  this->q = A * q;
  this->q = q / q.norm();
}
