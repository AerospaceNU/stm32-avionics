
#include <buzzer_heartbeat.h>
#include "state_cli_offload.h"
#include "data_offload.h"
#include "data_log.h" //This include is only needed to pass a flightID to userSetFlightID before command line implementation

void CliOffloadState::init() {

	/* Choosing flight ID number */
	userSetFlightID(data_log_get_last_flight_num());
}

EndCondition_t CliOffloadState::run() {
	buzzerHeartbeat();
	if (dataOffload())
		return EndCondition_t::CliCommandComplete;

	return EndCondition_t::NoChange;
}

void CliOffloadState::cleanup() {

}
