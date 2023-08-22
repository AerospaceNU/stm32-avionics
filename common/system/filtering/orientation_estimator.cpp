/*
 * orientation_estimator.cpp
 *
 *  Created on: Mar 21, 2022
 *      Author: sam
 */

#include "orientation_estimator.h"

#include <cmath>

#include "fcb_matrix.h"

#define G_THRESHOLD_MIN 9.0
#define G_THRESHOLD_MAX 11.0

OrientationEstimator::OrientationEstimator(float dt) : m_dt(dt) {}

OrientationEstimator::~OrientationEstimator() {}

void OrientationEstimator::reset() { 
  // this->q = Matrix<4, 1>({1.0, 0, 0, 0});
}

void OrientationEstimator::setDt(float dt) { this->m_dt = dt; }

void OrientationEstimator::setAccelVector(float rocket_acc_x,
                                          float rocket_acc_y,
                                          float rocket_acc_z) {
  Matrix<3, 1> a({rocket_acc_x, rocket_acc_y, rocket_acc_z});
  float a_norm = a.norm();
  if (!(a_norm > G_THRESHOLD_MIN && a_norm < G_THRESHOLD_MAX)) return;
  float ax = rocket_acc_x / a_norm;
  float ay = rocket_acc_y / a_norm;
  float az = rocket_acc_z / a_norm;
  float ex = atan2f(ay, az);
  float ey = atan2f(-ax, sqrtf(powf(ay, 2) + powf(az, 2)));
  float cx2 = cosf(ex / 2.0f);
  float sx2 = sinf(ex / 2.0f);
  float cy2 = cosf(ey / 2.0f);
  float sy2 = sinf(ey / 2.0f);
  this->q = ::Matrix<4, 1>({cx2 * cy2, sx2 * cy2, cx2 * sy2, -sx2 * sy2});
  this->q = q / q.norm();
}

void OrientationEstimator::update(float rocket_ang_vel_x,
                                  float rocket_ang_vel_y,
                                  float rocket_ang_vel_z) {
  Matrix<4, 4> omega(
      {0.0, -rocket_ang_vel_x, -rocket_ang_vel_y, -rocket_ang_vel_z,
       rocket_ang_vel_x, 0.0, rocket_ang_vel_z, -rocket_ang_vel_y,
       rocket_ang_vel_y, -rocket_ang_vel_z, 0.0, rocket_ang_vel_x,
       rocket_ang_vel_z, rocket_ang_vel_y, -rocket_ang_vel_x, 0.0});

  Matrix<3, 1> gyro({rocket_ang_vel_x, rocket_ang_vel_y, rocket_ang_vel_z});

  float w = gyro.norm();

  // special case -- w = 0, so omgmult becomes NaN and everything explodes
  // But w=0 means no rotation. So just return early
  if (fabs(w) < 1e-6) {
    return;
  }

  Matrix<4, 4> ident = (Matrix<4, 4>::identity() * cosf(w * m_dt / 2.0f));
  float omgmult = sinf(w * m_dt / 2.0f) / w;
  Matrix<4, 4> A = ident + omega * omgmult;
  this->q = A * q;
  this->q = q / q.norm();
}
