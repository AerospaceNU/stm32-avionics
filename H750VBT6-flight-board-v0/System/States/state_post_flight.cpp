
#include "state_post_flight.h"

#include "data_transmission.h"
#include "filters.h"
#include "hardware_manager.h"
#include "buzzerHeartbeat.h"

void PostFlightState::init() {
	// Empty
}

EndCondition_t PostFlightState::run() {
	buzzerHeartbeat();
	// Collect and transmit data
	HM_ReadSensorData();
	SensorData_t* sensorData = HM_GetSensorData();
	FilterData_t* filterData = getFilteredData();
	transmitData(sensorData, filterData, this->getID());

	// Detect if USB has been plugged in
	if (HM_UsbIsConnected()) {
		return EndCondition_t::UsbConnect;
	}
	return EndCondition_t::NoChange;
}

void PostFlightState::cleanup() {
	// Empty
}



