
#include "state_cli_config.h"

static CliConfigState::Configs_t configVals = {
	0, 	     // drogueCuts
	{0, 0, 0, 0}, 	     // drogueCutAltitudesM
	1000000, // mainCutAltitude
	0, 	     // groundElevationM
	14.85    // groundTemperatureC
};

void CliConfigState::init() {

}

EndCondition_t CliConfigState::run() {
	return EndCondition_t::NoChange;
}

void CliConfigState::cleanup() {

}

const CliConfigState::Configs_t* CliConfigState::getConfigs() {
	return &configVals;
}



