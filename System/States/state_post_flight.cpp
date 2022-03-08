#include "state_post_flight.h"

#include "buzzer_heartbeat.h"
#include "cli.h"
#include "data_log.h"
#include "data_transmission.h"
#include "state_log.h"
#include "filters.h"
#include "hardware_manager.h"

#define SIM_NO_DATA_TIMEOUT_MS 1000 // If no data comes in for this long, stop sim

void PostFlightState::init() {
	//Write that the flight is completed so that we don't jump back to a flight state
	state_log_write_complete();
	lastSimDataTime = HM_Millis();
}

EndCondition_t PostFlightState::run() {
	// Run buzzer heartbeat
	buzzerHeartbeat();

	// Check if data is still coming in (sim mode)
	uint32_t curTime = HM_Millis();
	if (HM_InSimMode()) {
		if (cbCount(cliGetRxBuffer()) >= sizeof(SensorData_t)) {
			lastSimDataTime = curTime;
		}
		if (curTime - lastSimDataTime > SIM_NO_DATA_TIMEOUT_MS) {
			return EndCondition_t::CliCommandComplete;
		}
	}

	// Collect and transmit data
	HM_ReadSensorData();
	SensorData_t* sensorData = HM_GetSensorData();
	filterApplyData(sensorData, HM_GetSensorProperties(), true);
	FilterData_t* filterData = filterGetData();

	// Log data at 1/10th rate for now in case this state is reached before it is supposed to
	if (this->getRunCounter() % 10 == 0)
		data_log_write(sensorData, filterData, this->getID());

	// Transmit at 1/100th rate
	if (this->getRunCounter() % 100 == 0)
		transmitData(sensorData, filterData, this->getID());

	// Detect if USB has been plugged in (non-sim mode)
	if (!HM_InSimMode() && HM_UsbIsConnected()) {
		return EndCondition_t::UsbConnect;
	}

	return EndCondition_t::NoChange;
}

void PostFlightState::cleanup() {
	// If in sim mode, finish it up and release any unread bytes
	if (HM_InSimMode()) {
		cliSendComplete(true, nullptr);
		HM_DisableSimMode();
		cbFlush(cliGetRxBuffer());
	}
}



