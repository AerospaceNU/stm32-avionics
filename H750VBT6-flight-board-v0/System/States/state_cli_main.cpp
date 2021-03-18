
#include "state_cli_main.h"

#include "hardware_manager.h"

void CliMainState::init() {
	HM_BuzzerSetFrequency(1000);
	HM_BuzzerStart();
}

EndCondition_t CliMainState::run() {
	// Just check USB for now
	// TODO: Check for valid configuration after CLI implemented
	if (!HM_UsbIsConnected()) {
		return EndCondition_t::UsbDisconnect;
	}
	return EndCondition_t::NoChange;
}

void CliMainState::cleanup() {
	HM_BuzzerStop();
}



