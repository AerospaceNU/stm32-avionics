
#include "state_cli_offload.h"
#include "buzzerHeartbeat.h"

void CliOffloadState::init() {

}

EndCondition_t CliOffloadState::run() {
	buzzerHeartbeat();
	return EndCondition_t::NoChange;
}

void CliOffloadState::cleanup() {

}



