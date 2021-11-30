
#include <buzzer_heartbeat.h>
#include "state_post_flight.h"

#include "data_log.h"
#include "data_transmission.h"
#include "filters.h"
#include "hardware_manager.h"

void PostFlightState::init() {
	// Empty
}

EndCondition_t PostFlightState::run() {
	// Run buzzer heartbeat
	buzzerHeartbeat();

	// Collect and transmit data
	HM_ReadSensorData();
	SensorData_t* sensorData = HM_GetSensorData();
	filterApplyData(sensorData);
	FilterData_t* filterData = filterGetData();

	// Log data at 1/10th rate for now in case this state is reached before it is supposed to
	if (this->getRunCounter() % 10 == 0)
		data_log_write(sensorData, filterData, this->getID());

	// Transmit at 1/100th rate
	if (this->getRunCounter() % 100 == 0)
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



