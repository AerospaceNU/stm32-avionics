
#include <buzzer_heartbeat.h>
#include "state_cli_choose_flight.h"

void CliChooseFlightState::init() {

}

EndCondition_t CliChooseFlightState::run() {
	buzzerHeartbeat();
	return EndCondition_t::NoChange;
}

void CliChooseFlightState::cleanup() {

}


