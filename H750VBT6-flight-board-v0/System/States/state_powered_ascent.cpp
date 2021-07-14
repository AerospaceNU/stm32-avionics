
#include "state_powered_ascent.h"

#include "hardware_manager.h"
#include "filters.h"
#include "data_log.h"
#include "data_transmission.h"

void PoweredAscentState::init() {
	maxAccelZ = 0;
}

EndCondition_t PoweredAscentState::run() {
	// Collect, filter, and log all sensor data
	HM_ReadSensorData();
	SensorData_t* sensorData = HM_GetSensorData();
	filterApplyData(sensorData);
	FilterData_t* filterData = filterGetData();
	data_log_write(sensorData, filterData, this->getID());

	// Transmit at 1/100th rate
	if (this->getRunCounter() % 100 == 0)
		transmitData(sensorData, filterData, this->getID());
	this->incrementRunCounter();

	maxAccelZ = filterData->acc_z > maxAccelZ ? filterData->acc_z : maxAccelZ;

	// Detect motor burnout if vertical acceleration changes by a certain amount
	if (maxAccelZ - filterData->acc_z > kMotorBurnoutZAccelDiffThreshold) {
		return EndCondition_t::MotorBurnout;
	}
	return EndCondition_t::NoChange;
}

void PoweredAscentState::cleanup() {
	// Empty
}



