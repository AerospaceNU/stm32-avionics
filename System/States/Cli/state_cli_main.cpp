
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

	// Check if USB is disconnected
	if (!HM_UsbIsConnected()) {
		return EndCondition_t::UsbDisconnect;
	}

	// Check for CLI input over USB
	CliCommand_t command = cliParse(CliComms_t::CLI_USB);

	switch (command) {
	case CliCommand_t::CALIBRATE:
		return EndCondition_t::CalibrateCommand;
	case CliCommand_t::CONFIG:
		return EndCondition_t::ConfigCommand;
	case CliCommand_t::ERASE_FLASH:
		return EndCondition_t::EraseFlashCommand;
	case CliCommand_t::HELP:
		return EndCondition_t::HelpCommand;
	case CliCommand_t::OFFLOAD:
		return EndCondition_t::OffloadCommand;
	case CliCommand_t::SIM:
		HM_EnableSimMode(cliGetRxBuffer());
		return EndCondition_t::SimCommand;
	case CliCommand_t::SENSE:
		return EndCondition_t::SenseCommand;
	case CliCommand_t::SHUTDOWN:
		return EndCondition_t::ShutdownCommand;
	default:
		break;
	}

	return EndCondition_t::NoChange;
}

void CliMainState::cleanup() {

}
