
#include "state_coast_ascent.h"

#include "data_log.h"
#include "data_transmission.h"
#include "filters.h"
#include "hardware_manager.h"

void CoastAscentState::init() {
	maxPosZ = 0;
	maxPosZTimeHit = HM_Millis();
}

EndCondition_t CoastAscentState::run() {
	// Collect, filter, log, and log all data
	HM_ReadSensorData();
	SensorData_t* sensorData = HM_GetSensorData();
	filterApplyData(sensorData);
	FilterData_t* filterData = filterGetData();
	data_log_write(sensorData, filterData, this->getID());

	// Transmit at 1/100th rate
	if (this->getRunCounter() % 100 == 0)
		transmitData(sensorData, filterData, this->getID());

	// Detect if new maximum Z position has been reached and record the time
	if (filterData->pos_z > maxPosZ) {
		maxPosZ = filterData->pos_z;
		maxPosZTimeHit = HM_Millis();
	}

	// Detect apogee if under max z position for specified amount of time
	if (filterData->pos_z < maxPosZ && HM_Millis() - maxPosZTimeHit > kTimeUnderApogeeThreshold) {
		if (++thresholdCounter > thresholdLimit) {
			return EndCondition_t::Apogee;
		}
	} else {
		thresholdCounter = 0;
	}
	return EndCondition_t::NoChange;
}

void CoastAscentState::cleanup() {
	// TODO: Transmit apogee
}



