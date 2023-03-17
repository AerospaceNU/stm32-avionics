
#include "state_ascent.h"

#include "cli_tasks.h"
#include "data_log.h"
#include "event_manager.h"
#include "filters.h"
#include "hardware_manager.h"
#include "state_log.h"

void AscentState::init() {
  // Write launched status
  dataLog_getFlightMetadata()->pressureRef = filter_getPressureRef();
  dataLog_getFlightMetadata()->gravityRef = filter_getGravityRef();
  dataLog_getFlightMetadata()->launched = 1;
  eventManager_setEventComplete(Event_e::launch);
  burnoutResetTimer = hm_millis();
  dataLog_writeFlightMetadata();
  maxPosZ = 0;
  stateLog_write(this->getID());
  CliTasks::configureForFlight();
}

EndCondition_e AscentState::run() {
  // Collect, filter, log, and log all data
  SensorData_s* sensorData = hm_getSensorData();
  FilterData_s* filterData = filter_getData();
  dataLog_write(sensorData, filterData, this->getID());

  // Detect if new maximum Z position has been reached and record the time
  if (filterData->pos_z_agl > maxPosZ) {
    maxPosZ = filterData->pos_z_agl;
  }

  // Detect burnout of acc_z drops
  if (filterData->world_acc_z < kBurnoutAccThreshold) {
    if (hm_millis() - burnoutResetTimer > kBurnoutResetTimeThreshold) {
      eventManager_setEventComplete(Event_e::burnout);
    }
  } else {
    burnoutResetTimer = hm_millis();
  }

  // Detect apogee if under max z position and negative velocity
  if (maxPosZ - filterData->pos_z_agl > kPosDiffThreshold &&
      filterData->world_vel_z < 0) {
    return EndCondition_e::Apogee;
  }

  return EndCondition_e::NoChange;
}

void AscentState::cleanup() {}
