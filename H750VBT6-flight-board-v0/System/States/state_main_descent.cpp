
#include "state_main_descent.h"

#include <math.h>
#include "data_log.h"
#include "data_transmission.h"
#include "filters.h"
#include "hardware_manager.h"

void MainDescentState::init() {
	// TODO: Deploy main parachute
	touchdownResetTime = HM_Millis();
}

EndCondition_t MainDescentState::run() {
	// Collect, filter, and log all sensor data
	HM_ReadSensorData();
	SensorData_t* sensorData = HM_GetSensorData();
	filterApplyData(sensorData);
	FilterData_t* filterData = filterGetData();
	data_log_write(sensorData, filterData, this->getID());

	// Transmit at 1/100th rate
	if (this->getRunCounter() % 100 == 0)
		transmitData(sensorData, filterData, this->getID());

	// Reset touchdown counting if outside acceleration threshold
	if (fabs(filterData->acc_z) > kTouchdownZAccelMagThreshold) {
		touchdownResetTime = HM_Millis();
	}

	// Detect touchdown if reset detection hasn't triggered for given amount of time
	if (HM_Millis() - touchdownResetTime > kTouchdownNoAccelTime) {
		return EndCondition_t::Touchdown;
	}
	return EndCondition_t::NoChange;
}

void MainDescentState::cleanup() {
	// Empty
}



