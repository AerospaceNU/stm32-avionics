
#include "state_coast_ascent.h"

#include "data_log.h"
#include "state_log.h"
#include "data_transmission.h"
#include "filters.h"
#include "hardware_manager.h"

void CoastAscentState::init() {
	transitionResetTimer = HM_Millis();
	maxPosZ = 0;
	state_log_write(this->getID());
}

EndCondition_t CoastAscentState::run() {
	// Collect, filter, log, and log all data
	HM_ReadSensorData();
	SensorData_t* sensorData = HM_GetSensorData();
	filterApplyData(sensorData, HM_GetSensorProperties(), false);
	FilterData_t* filterData = filterGetData();
	data_log_write(sensorData, filterData, this->getID());

	// Transmit at 1/100th rate
	if (this->getRunCounter() % 100 == 0)
		transmitData(sensorData, filterData, this->getID());

	// Detect if new maximum Z position has been reached and record the time
	if (filterData->pos_z > maxPosZ) {
		maxPosZ = filterData->pos_z;
	}

	// Detect apogee if under max z position for specified amount of time
	if (filterData->pos_z < maxPosZ) {
		if (HM_Millis() - transitionResetTimer > kTransitionResetTimeThreshold) {
			return EndCondition_t::Apogee;
		}
	} else {
		transitionResetTimer = HM_Millis();
	}

	return EndCondition_t::NoChange;
}

void CoastAscentState::cleanup() {
	// TODO: Transmit apogee
}



