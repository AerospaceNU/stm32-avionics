
#include "state_descent.h"

#include <cmath>

#include "data_log.h"
#include "filters.h"
#include "hardware_manager.h"
#include "state_log.h"

void DescentState::init() {
  transitionResetTimer = HM_Millis();
  altitude = 0;
  state_log_write(this->getID());

  sentCut1 = false;
  sentCut2 = false;
}

EndCondition_e DescentState::run() {
  // Collect, filter, and log all sensor data
  SensorData_s* sensorData = HM_GetSensorData();
  FilterData_s* filterData = filterGetData();
  data_log_write(sensorData, filterData, this->getID());

  // Reset touchdown threshold counter if recent change in z position is large
  // enough
  if (fabs(altitude - filterData->pos_z) > kTouchdownZPosChangeThreshold) {
    altitude = filterData->pos_z;
    transitionResetTimer = HM_Millis();
  } else {
    if (HM_Millis() - transitionResetTimer > kTransitionResetTimeThreshold) {
      return EndCondition_e::Touchdown;
    }
  }

  return EndCondition_e::NoChange;
}

void DescentState::cleanup() {
  // Empty
}
