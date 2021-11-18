
#include "state_drogue_descent_n.h"

#include "data_log.h"
#include "data_transmission.h"
#include "filters.h"
#include "hardware_manager.h"
#include "state_cli_config.h"

void DrogueDescentNState::init() {
	completeDrogueCuts_ = 0;
	// TODO: Actuate separation
}

EndCondition_t DrogueDescentNState::run() {
	// TODO: How to detect separation

	// Collect, filter, and log all data
	HM_ReadSensorData();
	SensorData_t* sensorData = HM_GetSensorData();
	filterApplyData(sensorData);
	FilterData_t* filterData = filterGetData();
	data_log_write(sensorData, filterData, this->getID());

	// Transmit at 1/100th rate
	if (this->getRunCounter() % 100 == 0)
		transmitData(sensorData, filterData, this->getID());

	// Detect if next drogue cut altitude has been reached
	if (completeDrogueCuts_ < CliConfigState::getConfigs()->drogueCuts) {
		if (filterData->pos_z < CliConfigState::getConfigs()->drogueCutAltitudesM[completeDrogueCuts_]) {
			// TODO: Actuate drogue cut
			completeDrogueCuts_ += 1;
		}
	}
	// Detect if main cut altitude has been reached
	else if (filterData->pos_z < CliConfigState::getConfigs()->mainCutAltitudeM) {
		if (++thresholdCounter > thresholdLimit) {
			return EndCondition_t::MainCutAltitude;
		}
	} else {
		thresholdCounter = 0;
	}
	return EndCondition_t::NoChange;
}

void DrogueDescentNState::cleanup() {

}



