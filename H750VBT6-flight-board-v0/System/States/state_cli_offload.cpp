
#include <buzzer_heartbeat.h>
#include "state_cli_offload.h"
#include "data_offload.h"

void CliOffloadState::init() {

	/* Offloading data and choosing flight ID number*/
	dataOffload();
	dataOffloadSetFLightID();
}

EndCondition_t CliOffloadState::run() {
	buzzerHeartbeat();
	return EndCondition_t::NoChange;
}

void CliOffloadState::cleanup() {

}
