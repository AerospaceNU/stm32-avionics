
#include "state_descent.h"

#include <cmath>

#include "data_log.h"
#include "filters.h"
#include "hardware_manager.h"
#include "state_log.h"
#include "trigger_manager.h"

void DescentState::init() {
  uint32_t apogeeTime = hm_millis();
  triggerManager_setApogeeTime(apogeeTime);
  dataLog_getFlightMetadata()->apogeeTimestamp = apogeeTime;
  dataLog_writeFlightMetadata();
  transitionResetTimer = hm_millis();
  altitude = 0;
  stateLog_write(this->getID());

  sentCut1 = false;
  sentCut2 = false;
}

EndCondition_e DescentState::run() {
  // Collect, filter, and log all sensor data
  SensorData_s* sensorData = hm_getSensorData();
  FilterData_s* filterData = filter_getData();
  dataLog_write(sensorData, filterData, this->getID());

  // Reset touchdown threshold counter if recent change in z position is large
  // enough
  if (fabs(altitude - filterData->pos_z_agl) > kTouchdownZPosChangeThreshold) {
    altitude = filterData->pos_z_agl;
    transitionResetTimer = hm_millis();
  } else {
    if (hm_millis() - transitionResetTimer > kTransitionResetTimeThreshold) {
      return EndCondition_e::Touchdown;
    }
  }

  return EndCondition_e::NoChange;
}

void DescentState::cleanup() {
  // Empty
}
