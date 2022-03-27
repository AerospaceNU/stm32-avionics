/*
 * flight_states_interface.cpp
 *
 *  Created on: Mar 12, 2022
 *      Author: matth
 */

#include "flight_state.h"
#include "hardware_manager.h"
#include "data_log.h"
#include "radio_manager.h"

EndCondition_t FlightState::run_state() {
	// Collect, filter, and log all sensor data
	HM_ReadSensorData();
	SensorData_t* sensorData = HM_GetSensorData();
	filterApplyData(sensorData, HM_GetSensorProperties(), m_hasPastApogee);
	FilterData_t* filterData = filterGetData();
	RadioManager_transmitData(sensorData, filterData, this->getID());

	EndCondition_t end = State::run_state();

	// We also should run periodic updates last
	HM_RadioUpdate();
	RadioManager_tick();

	return end;
}


