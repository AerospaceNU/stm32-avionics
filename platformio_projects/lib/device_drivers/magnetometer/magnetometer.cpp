//
// Created by nathan on 5/15/23.
//

#include "magnetometer.h"

Magnetometer::Magnetometer() = default;

std::array<double, 3> Magnetometer::getMagnetometerData() {
  return magnetometerData;
}

double Magnetometer::getMagnetometerFullScale() const {
  return fullScaleMagnetometer;
}
