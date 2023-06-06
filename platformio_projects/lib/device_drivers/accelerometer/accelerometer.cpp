//
// Created by nathan on 5/15/23.
//

#include "accelerometer.h"

Accelerometer::Accelerometer() = default;

std::array<double, 3> Accelerometer::getAccelerometerData() {
  return accelerometerData;
}

double Accelerometer::getAccelerometerFullScale() const {
  return fullScaleAccelerometer;
}
