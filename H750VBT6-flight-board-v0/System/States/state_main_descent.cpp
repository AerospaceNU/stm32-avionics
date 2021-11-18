
#include "state_main_descent.h"

#include <math.h>
#include "data_log.h"
#include "data_transmission.h"
#include "filters.h"
#include "hardware_manager.h"

void MainDescentState::init() {
	transitionResetTimer = HM_Millis();
	altitude = 0;
	// TODO: Deploy main parachute
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

	// Reset touchdown threshold counter if recent change in z position is large enough
	if (fabs(altitude - filterData->pos_z) > kTouchdownZPosChangeThreshold) {
		altitude = filterData->pos_z;
		transitionResetTimer = HM_Millis();
	} else {
		if (HM_Millis() - transitionResetTimer > kTransitionResetTimeThreshold) {
			return EndCondition_t::Touchdown;
		}
	}

	return EndCondition_t::NoChange;
}

void MainDescentState::cleanup() {
	// Empty
}



