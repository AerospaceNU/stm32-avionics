
#include "state_pre_flight.h"

#include <string.h>
#include "data_log.h"
#include "data_transmission.h"
#include "filters.h"
#include "hardware_manager.h"
#include "buzzer_report_scheme.h"

void PreFlightState::init() {
	data_log_assign_flight();
	minPosZ = 1000000; // Reset min position each new time pre-flight is reached
	// Set filter pressure reference to current pressure
	HM_ReadSensorData();
	filterSetPressureRef((HM_GetSensorData()->baro1_pres + HM_GetSensorData()->baro2_pres) / 2);
}

EndCondition_t PreFlightState::run() {

	// Produce a tone for each functioning peripheral
	buzzerReport();

	// Collect, and filter data
	HM_ReadSensorData();
	SensorData_t* sensorData = HM_GetSensorData();
	memcpy(&sensorDataBuffer[bufferCounter], sensorData, sizeof(SensorData_t));
	filterApplyData(sensorData);
	FilterData_t* filterData = filterGetData();
	memcpy(&filterDataBuffer[bufferCounter], filterData, sizeof(FilterData_t));

	// Transmit at 1/100th rate
	if (this->getRunCounter() % 100 == 0) {
		transmitData(sensorData, filterData, this->getID());
	}

	// Log at normal rate until launch detect is proven. TODO: Log when buffer is reset
	data_log_write(sensorData, filterData, this->getID());
	if (bufferCounter == 0) {
		// data_log_write(sensorData, filterData, this->getID());
	}

	// Increment and reset data buffer
	bufferCounter++;
	if (bufferCounter == kBufferSize)
		bufferCounter = 0;

	// Look to see if new minimum Z position has been reached
	if (filterData->pos_z < minPosZ) {
		minPosZ = filterData->pos_z;
		// Set pressure reference here because pressure may have "settled" after a bit
		filterSetPressureRef((sensorData->baro1_pres + sensorData->baro2_pres) / 2);
	}

	// Detect launch by looking for accel and z position difference thresholds
	if (filterData->acc_z > kLaunchAccelThreshold && filterData->pos_z - minPosZ > kLaunchPosZDiffThreshold) {
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



