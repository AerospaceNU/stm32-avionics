
#include "state_pre_flight.h"

#include <cstring>

#include "cli.h"
#include "cli_tasks.h"
#include "data_log.h"
#include "filters.h"
#include "hardware_manager.h"
#include "pyro_manager.h"

void PreFlightState::init() {
  transitionResetTimer = HM_Millis();
  prevPressureLogTime = HM_Millis();
  prefGravityRefTime = HM_Millis();

  // We only want to create a new flight in flash if
  // we came from Erase flash (offload, sim, etc shouldn't)
  if (!HM_InSimMode() &&
      (m_lastCliEndConn == EraseFlashCommand || m_lastCliEndConn == NoChange)) {
    data_log_assign_flight();
    m_lastCliEndConn = NoChange;
  }

  simModeStarted = false;
  gpsTimestamp = false;
  PyroManager_Init();
  filterInit(this->period_ms_ / 1000.0);
  HM_ReadSensorData();
  filterSetPressureRef(filterGetAveragePressure(HM_GetSensorData()));

  cli_tasks::ConfigureForGround();
  doCleanup = false;
}

EndCondition_e PreFlightState::run() {
  // Set GPS timestamp in metadata
  SensorData_s* sensorData = HM_GetSensorData();
#if HAS_DEV(GPS)
  if (!gpsTimestamp && sensorData->gpsData[0].timestamp > 0) {
    data_log_get_flight_metadata()->gpsTimestamp =
        sensorData->gpsData[0].timestamp;
    data_log_write_flight_metadata();
    gpsTimestamp = true;
  }
#endif  // HAS_DEV(GPS)

  memcpy(&sensorDataBuffer[bufferCounter], sensorData, sizeof(SensorData_s));
  FilterData_s* filterData = filterGetData();
  memcpy(&filterDataBuffer[bufferCounter], filterData, sizeof(FilterData_s));

  // Log at normal rate until launch detect is proven. TODO: Log when buffer is
  // reset
  // data_log_write(sensorData, filterData, this->getID());
  if (bufferCounter == 0) {
    data_log_write(sensorData, filterData, this->getID());
  }

  // Increment and reset data buffer
  bufferCounter++;
  if (bufferCounter == kBufferSize) bufferCounter = 0;

  // Recalculate ground pressure, gravity references periodically
  if (HM_Millis() - prevPressureLogTime > 1000) {
    prevPressureLogTime = HM_Millis();
    // Add a pressure ref to the filter
    filterAddPressureRef(sensorData);
  }
  if (HM_Millis() - prefGravityRefTime > kGravityRefInterval) {
    prefGravityRefTime = HM_Millis();
    filterAddGravityRef();
    filterAddGyroRef();
  }

  // Detect launch by looking for accel and z position difference thresholds
  // Either we should have large enough acceleration
  if (filterData->rocket_acc_x > kLaunchAccelThreshold) {
    if (HM_Millis() - transitionResetTimer > kTransitionResetTimeThreshold) {
      return EndCondition_e::Launch;
    }
  } else {
    transitionResetTimer = HM_Millis();
  }
  // Or a significantly large enough position change to override lack of
  // acceleration data
  double elevRef = cliGetConfigs()->groundElevationM;
  if (filterData->pos_z - elevRef > kLaunchPosZDiffFailsafeThreshold) {
    // This used to be in cleanup, but since it only happens via this
    // exit condition, it might be easier to live here

    // Write buffer onto data log. No need to add more code to stay in order
    // since timestamps exist It won't hurt to write if some buffer values
    // weren't filled
    for (int i = 0; i < kBufferSize; i++) {
      data_log_write(&sensorDataBuffer[i], &filterDataBuffer[i], this->getID());
    }
    data_log_get_flight_metadata()->pressureRef = filterGetPressureRef();
    data_log_write_flight_metadata();
    return EndCondition_e::Launch;
  }

  return EndCondition_e::NoChange;
}

void PreFlightState::cleanup() {}
