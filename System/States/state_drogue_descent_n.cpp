
#include "state_drogue_descent_n.h"

#include "cli.h"
#include "data_log.h"
#include "filters.h"
#include "hardware_manager.h"
#include "state_log.h"

void DrogueDescentNState::init() {
	transitionResetTimer = HM_Millis();
	completeDrogueCuts_ = 0;
	// TODO: Actuate separation
	state_log_write(this->getID());
}

EndCondition_t DrogueDescentNState::run() {
	// TODO: How to detect separation

	// Collect, filter, and log all data
	SensorData_t* sensorData = HM_GetSensorData();
	FilterData_t* filterData = filterGetData();
	data_log_write(sensorData, filterData, this->getID());

	// Detect if next drogue cut altitude has been reached
	if (completeDrogueCuts_ < cliGetConfigs()->drogueCuts) {
		if (filterData->pos_z < cliGetConfigs()->drogueCutAltitudesM[completeDrogueCuts_]) {
			// TODO: Actuate drogue cut
			completeDrogueCuts_ += 1;
		}
	}
	// Detect if main cut altitude has been reached
	else if (filterData->pos_z < cliGetConfigs()->mainCutAltitudeM) {
		if (HM_Millis() - transitionResetTimer > kTransitionResetTimeThreshold) {
			return EndCondition_t::MainCutAltitude;
		}
	} else {
		transitionResetTimer = HM_Millis();
	}
	return EndCondition_t::NoChange;
}

void DrogueDescentNState::cleanup() {

}



