//
// Created by nathan on 5/14/23.
//

#include "device_manager.h"

DeviceManager::DeviceManager() { Serial.begin(115200); }

bool DeviceManager::addBatteryMonitorAdc(int pin, double multiplier) {
  if (numBatteryMonitorADCs < batteryMonitorADCPins.size()) {
    batteryMonitorADCPins[numBatteryMonitorADCs] = pin;
    batteryAdcMultipliers[numBatteryMonitorADCs] = multiplier;
    numBatteryMonitorADCs++;
    return true;
  }

  return false;
}

bool DeviceManager::addRadio(Radio *radio) {
  if (!radios.full()) {
    radios.push_back(radio);
    return true;
  } else {
    return false;
  }
}

bool DeviceManager::addBarometer(Barometer *barometer) {
  if (!barometers.full()) {
    barometers.push_back(barometer);
    return true;
  } else {
    return false;
  }
}

bool DeviceManager::addGps(Gps *gps_ptr) {
  if (!gps.full()) {
    gps.push_back(gps_ptr);
    return true;
  } else {
    return false;
  }
}

bool DeviceManager::addImu(Imu *imu, bool has_mag) {
  if (!gyroscopes.full() && !accelerometers.full()) {
    gyroscopes.push_back(imu);
    accelerometers.push_back(imu);

    // TODO: Better handling of IMUs
    if (has_mag && !magnetometers.full()) {
      magnetometers.push_back(imu);
    }

    return true;
  } else {
    return false;
  }
}

bool DeviceManager::addMagnetometer(Magnetometer *mag) {
  if (!magnetometers.full()) {
    magnetometers.push_back(mag);
    return true;
  } else {
    return false;
  }
}

void DeviceManager::init() {
  for (auto &device : accelerometers) {
    devices.push_back(device);
  }

  for (auto &device : barometers) {
    devices.push_back(device);
  }

  for (auto &device : gps) {
    devices.push_back(device);
  }

  for (auto &device : gyroscopes) {
    devices.push_back(device);
  }

  for (auto &device : magnetometers) {
    devices.push_back(device);
  }

  for (auto &device : radios) {
    devices.push_back(device);
  }
}

void DeviceManager::readSensors() {
  for (auto &device : devices) {
    device->tick();
  }
}

uint DeviceManager::getNumberBatteryMonitorAdcs() const {
  return numBatteryMonitorADCs;
}

double DeviceManager::readBatteryVoltage(uint battery_num) {
  if (0 <= battery_num && battery_num < numBatteryMonitorADCs) {
    double raw_reading = analogRead(batteryMonitorADCPins[battery_num]);
    return raw_reading * batteryAdcMultipliers[battery_num];
  }

  return -1;
}

Accelerometer *DeviceManager::getAccelerometer(uint index) {
  if (0 <= index && index < accelerometers.size()) {
    return accelerometers[index];
  } else {
    return nullptr;
  }
}

Barometer *DeviceManager::getBarometer(uint index) {
  if (0 <= index && index < barometers.size()) {
    return barometers[index];
  } else {
    return nullptr;
  }
}

Gps *DeviceManager::getGps(uint index) {
  if (0 <= index && index < gps.size()) {
    return gps[index];
  } else {
    return nullptr;
  }
}

Gyroscope *DeviceManager::getGyroscope(uint index) {
  if (0 <= index && index < gyroscopes.size()) {
    return gyroscopes[index];
  } else {
    return nullptr;
  }
}

Magnetometer *DeviceManager::getMagnetometer(uint index) {
  if (0 <= index && index < magnetometers.size()) {
    return magnetometers[index];
  } else {
    return nullptr;
  }
}

Radio *DeviceManager::getRadio(uint index) {
  if (0 <= index && index < radios.size()) {
    return radios[index];
  } else {
    return nullptr;
  }
}

// Imu *DeviceManager::getImu(int index) {
//    if (0 <= index && index < imus.size()) {
//       return imus[index];
//    } else {
//       return nullptr;
//    }
// }
