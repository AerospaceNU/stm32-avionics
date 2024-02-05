//
// Created by nathan on 5/16/23.
//

#include "bno055_imu.h"

std::array<double, 3> adafruitVectorToArray(const sensors_vec_t& vector) {
  return {vector.x, vector.y, vector.z};
}

Bno055Imu::Bno055Imu() = default;

void Bno055Imu::init(TwoWire* wire, uint8_t address) {
  //    Need to construct the object here, so we do this weird thing to avoid
  //    dynamic memory allocation
  new (&driver_memory) Adafruit_BNO055(55, address, wire);
  bno055 = ((Adafruit_BNO055*)&driver_memory);

  // Init the sensor and turn off the internal fusion
  sensorStatus = bno055->begin();
  bno055->setMode(OPERATION_MODE_ACCGYRO);

  // TODO: Figure out full scale values
  fullScaleAccelerometer = 4 * 9.8;
  fullScaleGyro = 0;
  fullScaleMagnetometer = 0;
}

void Bno055Imu::tick() {
  // Read from the sensor
  sensors_event_t accelerometer_data, gyroscope_data, magnetometer_data;
  bno055->getEvent(&accelerometer_data, Adafruit_BNO055::VECTOR_ACCELEROMETER);
  bno055->getEvent(&gyroscope_data, Adafruit_BNO055::VECTOR_GYROSCOPE);
  bno055->getEvent(&magnetometer_data, Adafruit_BNO055::VECTOR_MAGNETOMETER);

  accelerometerData = adafruitVectorToArray(accelerometer_data.acceleration);
  gyroscopeData = adafruitVectorToArray(gyroscope_data.gyro);
  magnetometerData = adafruitVectorToArray(magnetometer_data.magnetic);

  for (double& i : magnetometerData) {
    i *= 0.01;  // Convert from uT to Gauss
  }
}
