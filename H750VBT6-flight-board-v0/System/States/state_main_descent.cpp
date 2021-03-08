
#include "state_main_descent.h"

#include <math.h>
#include "data_log.h"
#include "data_transmission.h"
#include "filters.h"
#include "hardware_manager.h"

void MainDescentState::init() {
	// TODO: Deploy main parachute
}

EndCondition_t MainDescentState::run() {
	// Collect, filter, log, and transmit all sensor data
	HM_ReadSensorData();
	SensorData_t* sensorData = HM_GetSensorData();
	applyFilterData(sensorData);
	FilterData_t* filterData = getFilteredData();
	data_log_write(sensorData, filterData, this->getID());
	transmitData(sensorData, filterData, this->getID());

	// Detect touchdown by looking for low vertical speed
	if (fabs(filterData->vel_z) < kTouchdownVelocityThreshold) {
		return EndCondition_t::Touchdown;
	}
	return EndCondition_t::NoChange;
}

void MainDescentState::cleanup() {
	// Empty
}



