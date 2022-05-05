/*
 * flight_states_interface.cpp
 *
 *  Created on: Mar 12, 2022
 *      Author: matth
 */

#include "flight_state.h"

#include "cli.h"
#include "cli_tasks.h"
#include "data_log.h"
#include "hardware_manager.h"
#include "pyro_manager.h"
#include "radio_manager.h"

EndCondition_t FlightState::m_lastCliEndConn;

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

  // Run the state
  EndCondition_t normalEndCondition = State::run_state();

  // CLI tick checks if we've gotten a new CLI command, and if we have, return
  // it
  auto cliEndCondition = cli_tasks::cliTick();
  if (cliEndCondition != NoChange) {
    // We'll get a cli end condition for things like Sim, Calibrate, Erase
    // But we only want to create a "new flight" if we exit with Erase
    m_lastCliEndConn = cliEndCondition;
    return cliEndCondition;
  }

  return normalEndCondition;
}
