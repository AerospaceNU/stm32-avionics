
#include "state_coast_ascent.h"

#include "data_log.h"
#include "data_transmission.h"
#include "filters.h"
#include "hardware_manager.h"

void CoastAscentState::init() {
	// Empty
}

EndCondition_t CoastAscentState::run() {
	// Collect, filter, log, and transmit all data
	HM_ReadSensorData();
	SensorData_t* sensorData = HM_GetSensorData();
	applyFilterData(sensorData);
	FilterData_t* filterData = getFilteredData();
	data_log_write(sensorData, filterData, this->getID());
	transmitData(sensorData, filterData, this->getID());

	// Detect apogee if z velocity becomes negative
	if (filterData->vel_z < 0) {
		return EndCondition_t::Apogee;
	}
	return EndCondition_t::NoChange;
}

void CoastAscentState::cleanup() {
	// TODO: Transmit apogee
}



