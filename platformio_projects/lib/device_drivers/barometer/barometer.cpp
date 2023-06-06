//
// Created by nathan on 5/15/23.
//

#include "barometer.h"

Barometer::Barometer() = default;

double Barometer::getPressureAtm() const { return pressureAtm; }

double Barometer::getTemperatureC() const { return temperatureC; }
