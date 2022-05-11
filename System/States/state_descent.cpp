
#include "state_descent.h"

#include <math.h>

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

EndCondition_t DescentState::run() {
  // Collect, filter, and log all sensor data
  SensorData_t* sensorData = HM_GetSensorData();
  FilterData_t* filterData = filterGetData();
  data_log_write(sensorData, filterData, this->getID());

  // If below X meters, disreef
  if (filterData->pos_z < 400 && !sentCut1) {
    HM_LineCuttersSendCut(1);
    sentCut1 = true;
  }
  if (filterData->pos_z < 200 && !sentCut2) {
    HM_LineCuttersSendCut(2);
    sentCut2 = true;
  }

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

void DescentState::cleanup() {
  // Empty
}
