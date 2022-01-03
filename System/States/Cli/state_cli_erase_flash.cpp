
#include "state_cli_erase_flash.h"

#include "cli.h"
#include "hardware_manager.h"

void CliEraseFlashState::init() {
	// Send initial ack to CLI and start erasing chip
	cliSendAck(true, nullptr);
	HM_FlashEraseChipStart();
}

EndCondition_t CliEraseFlashState::run() {
	if (HM_FlashIsEraseComplete()) {
		return EndCondition_t::CliCommandComplete;
	}
	return EndCondition_t::NoChange;
}

void CliEraseFlashState::cleanup() {
	// Send complete message to CLI. No way of knowing if there is a failure currently.
	cliSendComplete(true, nullptr);
}


