
#include "state_pre_flight.h"

#include <cstring>

#include "cli.h"
#include "cli_tasks.h"
#include "data_log.h"
#include "filters.h"
#include "hardware_manager.h"
#include "trigger_manager.h"

void PreFlightState::init() {
  transitionResetTimer = hm_millis();
  prevPressureLogTime = hm_millis();
  prefGravityRefTime = hm_millis();

  // We only want to create a new flight in flash if
  // we came from Erase flash (offload, sim, etc shouldn't)
  if (!hm_inSimMode() &&
      (m_lastCliEndConn == EraseFlashCommand || m_lastCliEndConn == NoChange)) {
    dataLog_assignFlight();
    m_lastCliEndConn = NoChange;
  }

  simModeStarted = false;
  gpsTimestamp = false;
  triggerManager_init();
  filter_init(this->period_ms_ / 1000.0);
  hm_readSensorData();
  filter_setPressureRef(filter_getAveragePressure(hm_getSensorData()));

  CliTasks::configureForGround();
  doCleanup = false;
}

EndCondition_e PreFlightState::run() {
  // Set GPS timestamp in metadata
  SensorData_s* sensorData = hm_getSensorData();
#if HAS_DEV(GPS)
  if (!gpsTimestamp && sensorData->gpsData[0].timestamp > 0) {
    dataLog_getFlightMetadata()->gpsTimestamp =
        sensorData->gpsData[0].timestamp;
    dataLog_writeFlightMetadata();
    gpsTimestamp = true;
  }
#endif  // HAS_DEV(GPS)

  memcpy(&sensorDataBuffer[bufferCounter], sensorData, sizeof(SensorData_s));
  FilterData_s* filterData = filter_getData();
  memcpy(&filterDataBuffer[bufferCounter], filterData, sizeof(FilterData_s));

  // Log at normal rate until launch detect is proven. TODO: Log when buffer is
  // reset
  // data_log_write(sensorData, filterData, this->getID());
  if (bufferCounter == 0) {
    dataLog_write(sensorData, filterData, this->getID());
  }

  // Increment and reset data buffer
  bufferCounter++;
  if (bufferCounter == kBufferSize) bufferCounter = 0;

  // Recalculate ground pressure, gravity references periodically
  if (hm_millis() - prevPressureLogTime > 1000) {
    prevPressureLogTime = hm_millis();
    // Add a pressure ref to the filter
    filter_addPressureRef(sensorData);
  }
  if (hm_millis() - prefGravityRefTime > kGravityRefInterval) {
    prefGravityRefTime = hm_millis();
    filter_addGravityRef();
    filter_addGyroRef();
  }

  // Detect launch by looking for accel and z position difference thresholds
  // Either we should have large enough acceleration
  if (filterData->rocket_acc_x > kLaunchAccelThreshold) {
    if (hm_millis() - transitionResetTimer > kTransitionResetTimeThreshold) {
      return EndCondition_e::Launch;
    }
  } else {
    transitionResetTimer = hm_millis();
  }
  // Or a significantly large enough position change to override lack of
  // acceleration data
  double elevRef = cli_getConfigs()->groundElevationM;
  if (filterData->pos_z - elevRef > kLaunchPosZDiffFailsafeThreshold) {
    // This used to be in cleanup, but since it only happens via this
    // exit condition, it might be easier to live here

    // Write buffer onto data log. No need to add more code to stay in order
    // since timestamps exist It won't hurt to write if some buffer values
    // weren't filled
    for (int i = 0; i < kBufferSize; i++) {
      dataLog_write(&sensorDataBuffer[i], &filterDataBuffer[i], this->getID());
    }
    dataLog_getFlightMetadata()->pressureRef = filter_getPressureRef();
    dataLog_writeFlightMetadata();
    return EndCondition_e::Launch;
  }

  return EndCondition_e::NoChange;
}

void PreFlightState::cleanup() {}
