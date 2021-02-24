
#include "state_cli_config.h"

static CliConfigState::Configs_t configVals = {
	0, // drogueCuts
	0, // drogueCutAltitudesM
	0 // mainCutAltitude
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



