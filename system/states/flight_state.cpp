/*
 * flight_states_interface.cpp
 *
 *  Created on: Mar 12, 2022
 *      Author: matth
 */

#include "flight_state.h"

#include "board_config_common.h"
#include "buzzer_heartbeat.h"
#include "cli.h"
#include "cli_tasks.h"
#include "data_log.h"
#include "hardware_manager.h"
#include "pyro_manager.h"
#include "radio_manager.h"

EndCondition_e FlightState::m_lastCliEndConn;

EndCondition_e FlightState::run_state() {
  // We also should run periodic updates
  HM_RadioUpdate();
  RadioManager_tick();
  HM_BleTick();

  // Collect, filter, and log all sensor data
  HM_ReadSensorData();
  SensorData_s* sensorData = HM_GetSensorData();
  filterApplyData(sensorData, HM_GetSensorProperties(), m_hasPastApogee);
  FilterData_s* filterData = filterGetData();

  // For now, transmit data to all attached radios
  for (int i = 0; i < NUM_RADIO; i++) {
    RadioManager_transmitData(i, sensorData, filterData, this->getID());
  }

  // Update pyros
  PyroManager_Update(filterData, m_hasPastApogee);

  // Run buzzer heartbeat
  buzzerHeartbeat();

  // Run the state
  EndCondition_e normalEndCondition = State::run_state();

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
