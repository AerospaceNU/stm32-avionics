
#include "state_powered_ascent.h"

#include "hardware_manager.h"
#include "filters.h"
#include "data_log.h"

void PoweredAscentState::init() {
	// Empty
}

EndCondition_t PoweredAscentState::run() {
	// Collect, filter, and log all sensor data
	HM_ReadSensorData();



	SensorData_t* sensorData = HM_GetSensorData();
	applyFilterData(sensorData);
	FilterData_t* filterData = getFilteredData();
	data_log_write(sensorData, filterData, this->getID());

	// Detect motor burnout if vertical acceleration changes by a certain amount
	if (filterData->jerk_z < kMotorBurnoutJerkThreshold) {
		return EndCondition_t::MotorBurnout;
	}
	return EndCondition_t::NoChange;
}

void PoweredAscentState::cleanup() {
	// Empty
}



