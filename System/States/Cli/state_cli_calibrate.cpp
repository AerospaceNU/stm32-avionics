
#include "state_cli_calibrate.h"

#include "cli.h"

void CliCalibrateState::init() {
	cliSendAck(true, nullptr);
}

EndCondition_t CliCalibrateState::run() {
	return EndCondition_t::CliCommandComplete;
}

void CliCalibrateState::cleanup() {
	cliSendComplete(true, nullptr);
}



