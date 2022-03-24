
#include "state_cli_config.h"

#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hardware_manager.h"

#include "cli.h"
#include "errno.h"
#include "board_config.h"

static void generateConfigHelp(const char* name, const char* value) {
	char msg[50 + 4];
	sprintf(msg, "%-30s %-20s\r\n", name, value);
	cliSend(msg);
}

void CliConfigState::init() {
	// No action
}

EndCondition_t CliConfigState::run() {
	// Get command line options
	CliOptionVals_t options = cliGetOptions();

	// Configure another drogue cut
	if (options.d) {
		// There must be more drogue cuts available
		if (cliGetConfigs()->drogueCuts >= MAX_DROGUE_CUTS) {
			cliSendAck(false, "Max supported drogue cuts reached");
			return EndCondition_t::CliCommandComplete;
		}
		// Try turning CLI argument into float
		char* endPtr;
		double cutCandidate = strtod(options.d, &endPtr);
		if (*endPtr != '\0') {
			cliSendAck(false, "Drogue cut invalid float");
			return EndCondition_t::CliCommandComplete;
		}
		// Set next available drogue cut and re-sort from highest to lowest altitude
		cliGetConfigs()->drogueCutAltitudesM[cliGetConfigs()->drogueCuts] = cutCandidate;
		cliGetConfigs()->drogueCuts++;
		qsort(cliGetConfigs()->drogueCutAltitudesM, cliGetConfigs()->drogueCuts, sizeof(double),
				[](const void* a, const void* b) {
			return (*(double*)b > *(double*)a) ? 1 : (*(double*)b < *(double*)a) ? -1 : 0;
		});
	}

	// Configure main cut
	if (options.m) {
		char* endPtr;
		double altitude = strtod(options.m, &endPtr);
		if (*endPtr != '\0') {
			cliSendAck(false, "Main cut altitude invalid float");
			return EndCondition_t::CliCommandComplete;
		}
		cliGetConfigs()->mainCutAltitudeM = altitude;
	}

	// Configure ground elevation
	if (options.e) {
		char* endPtr;
		double elevation = strtod(options.e, &endPtr);
		if (*endPtr != '\0') {
			cliSendAck(false, "Ground elevation invalid float");
			return EndCondition_t::CliCommandComplete;
		}
		cliGetConfigs()->groundElevationM = elevation;
	}

	// Configure ground temperature
	if (options.t) {
		char* endPtr;
		double temperature = strtod(options.t, &endPtr);
		if (*endPtr != '\0') {
			cliSendAck(false, "Ground temperature invalid float");
			return EndCondition_t::CliCommandComplete;
		}
		cliGetConfigs()->groundTemperatureC = temperature;
	}

	// Configure radio channel
	if (options.c) {
		char* endPtr;
		errno = 0;
		int channel = strtol(options.c, &endPtr, 0);
		if (*endPtr != '\0' || (errno != 0 && channel == 0)) {
			cliSendAck(false, "Invalid channel integer");
			return EndCondition_t::CliCommandComplete;
		}
		HM_RadioSetChannel(TELEMETRY_RADIO, channel);
	}

	// Send positive ACK (all inputs have been appropriately processed)
	cliSendAck(true, nullptr);

	// Send help message to cli
	if (options.h) {
		char name[30];
		char val[20];
		// New line
		cliSend("\r\n");
		// Drogue cut
		for (int i = 0; i < cliGetConfigs()->drogueCuts; i++) {
			sprintf(name, "Drogue Cut %i Altitude (m):", i + 1);
			sprintf(val, "%.3f", cliGetConfigs()->drogueCutAltitudesM[i]);
			generateConfigHelp(name, val);
		}
		// Main cut
		sprintf(val, "%.3f", cliGetConfigs()->mainCutAltitudeM);
		generateConfigHelp("Main Cut Altitude (m):", val);
		// Ground elevation
		sprintf(val, "%.3f", cliGetConfigs()->groundElevationM);
		generateConfigHelp("Ground Elevation (m):", val);
		// Ground temperature
		sprintf(val, "%.3f", cliGetConfigs()->groundTemperatureC);
		generateConfigHelp("Ground Temperature (C):", val);
	}

	// If reached, send complete message to CLI
	cliSendComplete(true, nullptr);
	return EndCondition_t::CliCommandComplete;
}

void CliConfigState::cleanup() {
	// No action
}
