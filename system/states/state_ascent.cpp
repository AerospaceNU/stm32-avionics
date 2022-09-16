
#include "state_ascent.h"

#include "cli_tasks.h"
#include "data_log.h"
#include "filters.h"
#include "hardware_manager.h"
#include "state_log.h"
#include "trigger_manager.h"

void AscentState::init() {
  // Write launched status
  data_log_get_flight_metadata()->launched = 1;
  data_log_write_flight_metadata();
  maxPosZ = 0;
  state_log_write(this->getID());
  cli_tasks::ConfigureForFlight();
}

EndCondition_e AscentState::run() {
  // Collect, filter, log, and log all data
  SensorData_s* sensorData = HM_GetSensorData();
  FilterData_s* filterData = filterGetData();
  data_log_write(sensorData, filterData, this->getID());

  // Detect if new maximum Z position has been reached and record the time
  if (filterData->pos_z > maxPosZ) {
    maxPosZ = filterData->pos_z;
  }

  // Detect apogee if under max z position and negative velocity
  if (maxPosZ - filterData->pos_z > kPosDiffThreshold &&
      filterData->rocket_vel_x < 0) {
    return EndCondition_e::Apogee;
  }

  return EndCondition_e::NoChange;
}

void AscentState::cleanup() { TriggerManager_SetApogeeTime(HM_Millis()); }
