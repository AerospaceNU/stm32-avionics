
#include "state_main_descent.h"

#include <math.h>

#include "data_log.h"
#include "filters.h"
#include "hardware_manager.h"
#include "state_log.h"

void MainDescentState::init() {
  transitionResetTimer = HM_Millis();
  altitude = 0;
  // TODO: Deploy main parachute
  state_log_write(this->getID());
}

EndCondition_t MainDescentState::run() {
  // Collect, filter, and log all sensor data
  SensorData_t* sensorData = HM_GetSensorData();
  FilterData_t* filterData = filterGetData();
  data_log_write(sensorData, filterData, this->getID());

  // Reset touchdown threshold counter if recent change in z position is large
  // enough
  if (fabs(altitude - filterData->pos_z) > kTouchdownZPosChangeThreshold) {
    altitude = filterData->pos_z;
    transitionResetTimer = HM_Millis();
  } else {
    if (HM_Millis() - transitionResetTimer > kTransitionResetTimeThreshold) {
      return EndCondition_t::Touchdown;
    }
  }

  return EndCondition_t::NoChange;
}

void MainDescentState::cleanup() {
  // Empty
}
