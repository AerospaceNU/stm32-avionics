#pragma once

#include "data_structures.h"

/**
 * @brief Supported sensor status results.
 */
enum class SensorStatus {
  /// Sensor is not valid or could not be found.
  INVALID,
  /// Sensor is connected and working properly.
  ONLINE,
  /// Sensor has fullscaled, i.e. is still functioning but has maxed out the
  /// output range.
  FULLSCALE,
  /// Sensor has disconnected but was previously working
  DISCONNECTED
};

/**
 * @brief Supported status codes from a sensor update event.
 */
enum class UpdateStatus {
  /// Sensor data has not changed.
  NO_CHANGE,
  /// Sensor data has been updated.
  NEW_DATA
};

/**
 * @brief Interface for all sensor devices representing basic usage.
 *
 * @tparam TDataType the datatype reported by the sensor.
 */
template <typename TDataType>
class ISensor {
 public:
  // Using directive to allow DataType to be accessible inside the class
  using DataType = TDataType;

  /**
   * @brief Get the current status of the sensor.
   *
   * @returns current SensorStatus of the sensor.
   */
  virtual const SensorStatus getStatus() const = 0;

  /**
   * @brief Update the sensor and possibly receive new data.
   *
   * @param[out] dataOut Data output from the sensor.
   *
   * @returns the status of the sensor update and if new data is available.
   *          If result is UpdateStatus::NEW_DATA, dataOut will be filled with
   *          updated sensor data, and will be untouched otherwise.
   */
  virtual UpdateStatus update(DataType& dataOut) = 0;
};

using Barometer = ISensor<BarometerData_s>;
