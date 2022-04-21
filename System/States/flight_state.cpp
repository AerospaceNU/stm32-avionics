/*
 * flight_states_interface.cpp
 *
 *  Created on: Mar 12, 2022
 *      Author: matth
 */

#include "flight_state.h"

#include "data_log.h"
#include "hardware_manager.h"
#include "pyro_manager.h"
#include "radio_manager.h"

EndCondition_t FlightState::run_state() {
  // We also should run periodic updates
  HM_RadioUpdate();
  RadioManager_tick();

  // Collect, filter, and log all sensor data
  HM_ReadSensorData();
  SensorData_t* sensorData = HM_GetSensorData();
  filterApplyData(sensorData, HM_GetSensorProperties(), m_hasPastApogee);
  FilterData_t* filterData = filterGetData();
  RadioManager_transmitData(sensorData, filterData, this->getID());

  // Update pyros
  PyroManager_Update(filterData, m_hasPastApogee);

  EndCondition_t end = State::run_state();

  return end;
}
