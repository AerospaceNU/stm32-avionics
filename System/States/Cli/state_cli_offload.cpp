#include "state_cli_offload.h"

#include <stdio.h>
#include <stdlib.h>

#include "buzzer_heartbeat.h"
#include "cli.h"
#include "data_offload.h"
#include "data_log.h" //This include is only needed to pass a flightID to userSetFlightID before command line implementation

void CliOffloadState::init() {
	CliOptionVals_t cliOptions = cliGetOptions();

	// If specific flight number is provided, try offloading that
	if (cliOptions.f) {
		uint32_t flightNum = (uint32_t) atoi(cliOptions.f);
		// Flight number 0 explicitly doesn't exist, so 0 means atoi failed
		if (flightNum == 0) {
			initSuccess_ = false;
			cliSendAck(false, "Flight number invalid integer or 0");
			return;
		}
		if (flightNum > data_log_get_last_flight_num()) {
			initSuccess_ = false;
			cliSendAck(false, "Flight number doesn't exist");
			return;
		}
		dataOffloadSetFlightId(flightNum);
	}

	// Otherwise, offload most recent flight
	else {
		dataOffloadSetFlightId(data_log_get_last_flight_num());
	}

	// Send ack to command line if reached
	initSuccess_ = true;
	cliSendAck(true, nullptr);
}

EndCondition_t CliOffloadState::run() {
	// If initialization didn't succeed, stop
	if (!initSuccess_) {
		return CliCommandComplete;
	}

	buzzerHeartbeat();

	// If help command, send help and stop
	if (cliGetOptions().h) {
		char sendString[50];
		uint32_t lastFlightNum = data_log_get_last_flight_num();
		if (lastFlightNum == 0) {
			sprintf(sendString, "\r\nNo flights exist to offload\r\n");
		}
		else {
			sprintf(sendString, "\r\nOffload a flight number between 1 and %lu\r\n", lastFlightNum);
		}
		cliSend(sendString);
		return EndCondition_t::CliCommandComplete;
	}

	// Offload one chunk of data
	if (dataOffload()) {
		return EndCondition_t::CliCommandComplete;
	}

	return EndCondition_t::NoChange;
}

void CliOffloadState::cleanup() {
	// Only send complete message if initialization was successful
	if (initSuccess_) {
		cliSendComplete(true, nullptr);
	}
}
