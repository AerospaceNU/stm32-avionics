//
// Created by nathan on 5/16/23.
//

#include "mpl115a2_barometer.h"

Mpl115a2Barometer::Mpl115a2Barometer() = default;

void Mpl115a2Barometer::init() { sensorStatus = mpl115A2.begin(); }

void Mpl115a2Barometer::tick() {
  float pressure_kpa;
  float temperature_c;

  mpl115A2.getPT(&pressure_kpa, &temperature_c);

  temperatureC = (double)temperature_c;
  pressureAtm = pressure_kpa / 101.325;
}
