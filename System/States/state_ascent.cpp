
#include "state_ascent.h"

#include "data_log.h"
#include "state_log.h"
#include "filters.h"
#include "hardware_manager.h"

void AscentState::init() {
	data_log_set_launched_metadata(); // Write launched status
	data_log_write_metadata();
	maxPosZ = 0;
	state_log_write(this->getID());
}

EndCondition_t AscentState::run() {
	// Collect, filter, log, and log all data
	SensorData_t* sensorData = HM_GetSensorData();
	FilterData_t* filterData = filterGetData();
	data_log_write(sensorData, filterData, this->getID());

	// Detect if new maximum Z position has been reached and record the time
	if (filterData->pos_z > maxPosZ) {
		maxPosZ = filterData->pos_z;
	}

	// Detect apogee if under max z position and negative velocity
	if (maxPosZ - filterData->pos_z > kPosDiffThreshold && filterData->vel_z < 0) {
		return EndCondition_t::Apogee;
	}

	return EndCondition_t::NoChange;
}

void AscentState::cleanup() {
	// TODO: Transmit apogee
}



