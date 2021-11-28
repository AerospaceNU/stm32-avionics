
#include <buzzer_heartbeat.h>
#include "state_cli_main.h"

#include "hardware_manager.h"

void CliMainState::init() {

}

EndCondition_t CliMainState::run() {
	buzzerHeartbeat();
	// Just check USB for now
	// TODO: Check for valid configuration after CLI implemented
	if (!HM_UsbIsConnected()) {
		return EndCondition_t::UsbDisconnect;
	}
	return EndCondition_t::NoChange;
}

void CliMainState::cleanup() {

}


