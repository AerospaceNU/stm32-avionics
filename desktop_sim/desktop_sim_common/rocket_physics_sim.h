#pragma once

#include <frc/geometry/Pose3d.h>
#include <units/time.h>

#include <Eigen/Core>

#include "sim_timing.h"

/**
 * @brief Dumb rocket 6dof kinematics simulator. World is +X forward, +Y left,
 * +Z up local is +X along the rocket axis, +y and +z normal to that.
 * https://gitlab.com/aeronu/dollar-per-foot/stm32-avionics/-/wikis/FCB-coordinate-system
 *
 */
class RocketPhysicsSim {
 public:
  frc::Pose3d rocketPose_world;
  Eigen::Vector3d rocketVelocity_world;

  void Update(Eigen::Vector3d angularVelocity_world,
              Eigen::Vector3d accelerationWorld, units::second_t dt) {
    // TODO is this valid?

    // first step position
    auto deltaPos_world =
        rocketVelocity_world * dt.to<double>() +
        dt.to<double>() * dt.to<double>() / 2 * accelerationWorld;

    // then step orientation?

    double rocket_ang_vel_x = angularVelocity_local(0);
    double rocket_ang_vel_y = angularVelocity_local(1);
    double rocket_ang_vel_z = angularVelocity_local(2);
    Eigen::Matrix4d omega;
    omega << 0.0, -rocket_ang_vel_x, -rocket_ang_vel_y, -rocket_ang_vel_z,
        rocket_ang_vel_x, 0.0, rocket_ang_vel_z, -rocket_ang_vel_y,
        rocket_ang_vel_y, -rocket_ang_vel_z, 0.0, rocket_ang_vel_x,
        rocket_ang_vel_z, rocket_ang_vel_y, -rocket_ang_vel_x, 0.0;
    Eigen::Vector3d gyro(
        {rocket_ang_vel_x, rocket_ang_vel_y, rocket_ang_vel_z});

    float w = gyro.norm();

    // special case -- w = 0, so omgmult becomes NaN and everything explodes
    // But w=0 means no rotation. So just return early
    if (fabs(w) < 1e-6) {
      return;
    }

    // Matrix<4, 4> ident = (Matrix<4, 4>::identity() * cos(w * m_dt / 2.0));
    // float omgmult = sin(w * m_dt / 2.0) / w;
    // Matrix<4, 4> A = ident + omega * omgmult;
    // this->q = A * q;
    // this->q = q / q.norm();
    // }
  }
};