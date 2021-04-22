
#include "state_pre_flight.h"

#include <string.h>
#include "data_log.h"
#include "data_transmission.h"
#include "filters.h"
#include "hardware_manager.h"
#include "buzzer_heartbeat.h"
#include "buzzer_report_scheme.h"

void PreFlightState::init() {
	data_log_assign_flight();
}

EndCondition_t PreFlightState::run() {
	if (reportCount < 300){
		buzzerReport();
		reportCount++;
	} else{
		buzzerHeartbeat();
	}
	// Collect, and filter data
	HM_ReadSensorData();
	SensorData_t* sensorData = HM_GetSensorData();
	memcpy(&sensorDataBuffer[bufferCounter], sensorData, sizeof(SensorData_t));
	applyFilterData(sensorData);
	FilterData_t* filterData = getFilteredData();
	memcpy(&filterDataBuffer[bufferCounter], filterData, sizeof(FilterData_t));

	// Log and transmit at 1/10th rate
	if (bufferCounter == 0) {
		data_log_write(sensorData, filterData, this->getID());
		transmitData(sensorData, filterData, this->getID());
	}

	// Increment buffer
	bufferCounter++;
	if (bufferCounter == kBufferSize)
		bufferCounter = 0;

	// Detect launch by looking for velocity and accel thresholds
	if (filterData->acc_z > kLaunchAccelThreshold && filterData->vel_z > kLaunchVelThreshold) {
		return EndCondition_t::Launch;
	}
	// Detect if USB was plugged back in
	if (HM_UsbIsConnected()) {
		return EndCondition_t::UsbConnect;
	}

	return EndCondition_t::NoChange;
}

void PreFlightState::cleanup() {
	// Write buffer onto data log. No need to add more code to stay in order since timestamps exist
	// It won't hurt to write if some buffer values weren't filled or if USB was plugged back in
	for (int i = 0; i < kBufferSize; i++) {
		data_log_write(&sensorDataBuffer[i], &filterDataBuffer[i], this->getID());
	}
}



