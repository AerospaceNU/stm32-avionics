//
// Created by nathan on 5/16/23.
//

#include "base_device.h"

BaseDevice::BaseDevice() = default;

bool BaseDevice::getSensorStatus() const { return sensorStatus; }

void BaseDevice::tick() {}
