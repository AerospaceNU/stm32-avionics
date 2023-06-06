//
// Created by nathan on 5/15/23.
//

#include "gyroscope.h"

Gyroscope::Gyroscope() = default;

std::array<double, 3> Gyroscope::getGyroscopeData() { return gyroscopeData; }

double Gyroscope::getGyroscopeFullScale() const { return fullScaleGyro; }
