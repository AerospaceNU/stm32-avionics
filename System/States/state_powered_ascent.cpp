
#include "state_powered_ascent.h"

#include "hardware_manager.h"
#include "filters.h"
#include "data_log.h"
#include "state_log.h"
#include "data_transmission.h"

void PoweredAscentState::init() {
	transitionResetTimer = HM_Millis();
	maxAccelZ = 0;
	state_log_write(this->getID());
}

EndCondition_t PoweredAscentState::run() {
	// Collect, filter, and log all sensor data
	HM_ReadSensorData();
	SensorData_t* sensorData = HM_GetSensorData();
	filterApplyData(sensorData, false);
	FilterData_t* filterData = filterGetData();
	data_log_write(sensorData, filterData, this->getID());

	// Transmit at 1/100th rate
	if (this->getRunCounter() % 100 == 0)
		transmitData(sensorData, filterData, this->getID());

	maxAccelZ = filterData->acc_z > maxAccelZ ? filterData->acc_z : maxAccelZ;

	// Detect motor burnout if vertical acceleration changes by a certain amount
	if (maxAccelZ - filterData->acc_z > kMotorBurnoutZAccelDiffThreshold) {
		if (HM_Millis() - transitionResetTimer > kTransitionResetTimeThreshold) {
			return EndCondition_t::MotorBurnout;
		}
	} else {
		transitionResetTimer = HM_Millis();
	}
	return EndCondition_t::NoChange;
}

void PoweredAscentState::cleanup() {
	// Empty
}



