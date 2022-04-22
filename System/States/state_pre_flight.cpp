
#include "state_pre_flight.h"

#include <string.h>

#include "buzzer_report_scheme.h"
#include "cli.h"
#include "data_log.h"
#include "filters.h"
#include "hardware_manager.h"
#include "pyro_manager.h"

void PreFlightState::init() {
  transitionResetTimer = HM_Millis();
  prevPressureLogTime = HM_Millis();
  prefGravityRefTime = HM_Millis();
  if (!HM_InSimMode()) data_log_assign_flight();
  simModeStarted = false;
  gpsTimestamp = false;
  PyroManager_Init();
  filterInit(this->period_ms_ / 1000.0);
  HM_ReadSensorData();
  filterSetPressureRef(
      (HM_GetSensorData()->baro1_pres + HM_GetSensorData()->baro2_pres) / 2);
}

EndCondition_t PreFlightState::run() {
  // Produce a tone for each functioning peripheral
  buzzerReport();

  // Collect, and filter data
  SensorData_t* sensorData = HM_GetSensorData();
  if (!gpsTimestamp && sensorData->gps_timestamp > 0) {
    data_log_get_flight_metadata()->gpsTimestamp = sensorData->gps_timestamp;
    data_log_write_flight_metadata();
    gpsTimestamp = true;
  }

  memcpy(&sensorDataBuffer[bufferCounter], sensorData, sizeof(SensorData_t));
  FilterData_t* filterData = filterGetData();
  memcpy(&filterDataBuffer[bufferCounter], filterData, sizeof(FilterData_t));

  // Log at normal rate until launch detect is proven. TODO: Log when buffer is
  // reset
  // data_log_write(sensorData, filterData, this->getID());
  if (bufferCounter == 0) {
    data_log_write(sensorData, filterData, this->getID());
  }

  // Increment and reset data buffer
  bufferCounter++;
  if (bufferCounter == kBufferSize) bufferCounter = 0;

  // Check pressures once per second
  if (HM_Millis() - prevPressureLogTime > 1000) {
    prevPressureLogTime = HM_Millis();
    // Add a pressure ref to the filter
    filterAddPressureRef(
        (HM_GetSensorData()->baro1_pres + HM_GetSensorData()->baro2_pres) / 2);
  }
  if (HM_Millis() - prefGravityRefTime > 1000) {
    prefGravityRefTime = HM_Millis();
    filterAddGravityRef();
  }

  // Detect launch by looking for accel and z position difference thresholds
  // Either we should have large enough acceleration
  if (filterData->acc_z > kLaunchAccelThreshold) {
    if (HM_Millis() - transitionResetTimer > kTransitionResetTimeThreshold) {
      return EndCondition_t::Launch;
    }
  } else {
    transitionResetTimer = HM_Millis();
  }
  // Or a significantly large enough position change to override lack of
  // acceleration data
  double elevRef = cliGetConfigs()->groundElevationM;
  if (filterData->pos_z - elevRef > kLaunchPosZDiffFailsafeThreshold) {
    return EndCondition_t::Launch;
  }

  // Detect if USB was plugged back in (not in sim mode)
  if (!HM_InSimMode() && HM_UsbIsConnected()) {
    return EndCondition_t::UsbConnect;
  }

  return EndCondition_t::NoChange;
}

void PreFlightState::cleanup() {
  // Write buffer onto data log. No need to add more code to stay in order since
  // timestamps exist It won't hurt to write if some buffer values weren't
  // filled or if USB was plugged back in
  for (int i = 0; i < kBufferSize; i++) {
    data_log_write(&sensorDataBuffer[i], &filterDataBuffer[i], this->getID());
  }
  data_log_get_flight_metadata()->pressureRef = filterGetPressureRef();
  data_log_write_flight_metadata();
}
