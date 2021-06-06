
#include "state_cli_main.h"

#include "buzzer_heartbeat.h"
#include "hardware_manager.h"
#include "cli.h"

void CliMainState::init() {
	cliInit();
}

EndCondition_t CliMainState::run() {
	// Run buzzer heartbeat
	buzzerHeartbeat();

	// Check for CLI input over USB
	CliCommand_t command = cliParse(CliComms_t::CLI_USB);

	// Just check USB for now
	// TODO: Check for valid configuration after CLI implemented
	//if (!HM_UsbIsConnected()) {
	//	return EndCondition_t::UsbDisconnect;
	//}
	return EndCondition_t::NoChange;
}

void CliMainState::cleanup() {

}



