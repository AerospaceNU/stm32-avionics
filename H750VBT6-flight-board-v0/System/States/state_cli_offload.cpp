
#include <buzzer_heartbeat.h>
#include "state_cli_offload.h"

void CliOffloadState::init() {

}

EndCondition_t CliOffloadState::run() {
	buzzerHeartbeat();
	return EndCondition_t::NoChange;
}

void CliOffloadState::cleanup() {

}



