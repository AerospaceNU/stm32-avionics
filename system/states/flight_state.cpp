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
#include "radio_manager.h"
#include "trigger_manager.h"

EndCondition_e FlightState::m_lastCliEndConn;

EndCondition_e FlightState::runState() {
  // We also should run periodic updates
  hm_radioUpdate();
  radioManager_tick();
  hm_bleTick();

  // Collect, filter, and log all sensor data
  hm_readSensorData();
  SensorData_s* sensorData = hm_getSensorData();
  filter_applyData(sensorData, hm_getSensorProperties(), m_hasPassedApogee);
  FilterData_s* filterData = filter_getData();

  // For now, transmit data to all attached radios
  for (int i = 0; i < NUM_RADIO; i++) {
    radioManager_transmitData(i, sensorData, filterData, this->getID());
  }

  // Update pyros
  triggerManager_update(filterData);

  // Run buzzer heartbeat
  buzzerHeartbeat_tick();

  // Run the state
  EndCondition_e normalEndCondition = State::runState();

  // CLI tick checks if we've gotten a new CLI command, and if we have, return
  // it
  auto cliEndCondition = CliTasks::tick();
  if (cliEndCondition != NoChange) {
    // We'll get a cli end condition for things like Sim, Calibrate, Erase
    // But we only want to create a "new flight" if we exit with Erase
    m_lastCliEndConn = cliEndCondition;
    return cliEndCondition;
  }

  return normalEndCondition;
}
