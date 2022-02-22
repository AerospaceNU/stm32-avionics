/*
 * cli.c
 */

#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "cli.h"
#include "hardware_manager.h"

#define INPUT_BUFFER_SIZE 2048
#define MAX_ARGS 50

static char inputBuffer[INPUT_BUFFER_SIZE + 1];  // +1 accounts for null terminator required for strtok

static CliOptionVals_t cliOptionVals = {
		.d = NULL,
		.e = NULL,
		.t = NULL,
		.f = NULL,
		.m = NULL,
		.h = false
};

static int primaryCommand = 0;

static struct option longOptions[] = {
		{"calibrate", no_argument, &primaryCommand, CALIBRATE},
		{"config", no_argument, &primaryCommand, CONFIG},
		{"erase", no_argument, &primaryCommand, ERASE_FLASH},
		{"help", no_argument, &primaryCommand, HELP},
		{"offload", no_argument, &primaryCommand, OFFLOAD},
		{"sim", no_argument, &primaryCommand, SIM},
		{"sense", no_argument, &primaryCommand, SENSE},
		{"shutdown", no_argument, &primaryCommand, SHUTDOWN},
		{0, 0, 0, 0}
};

static CliConfigs_t cliConfigs = {
	0, 	     	// drogueCuts
	{0},		// drogueCutAltitudesM
	1000000, 	// mainCutAltitude
	0, 	     	// groundElevationM
	14.85    	// groundTemperatureC
};

static CliComms_t lastCommsType; // Used to help send ack to right places

void cliInit() {
	opterr = 0; // Don't print any messages to standard error stream since this is embedded device
}

CliConfigs_t* cliGetConfigs() {
	return &cliConfigs;
}

CliCommand_t cliParse(CliComms_t commsType) {
	// Set last comms type
	lastCommsType = commsType;

	// Get buffer from hardware manager
	uint32_t bytesRead = 0; // Raw bytes read from hardware manager to be discarded, eventually including \r\n
	switch (commsType) {
	case CLI_BLUETOOTH:
		break;
	case CLI_RADIO:
		break;
	case CLI_USB: {
		// Read buffer, flush if full (likely bad inputs), and copy to input buffer
		bytesRead = HM_UsbGetRxBuffer()->count; // Each element 1 byte
		if (bytesRead == HM_UsbGetRxBuffer()->capacity) {
			cbFlush(HM_UsbGetRxBuffer());
			bytesRead = 0;
		}
		cbPeek(HM_UsbGetRxBuffer(), inputBuffer, NULL);
		break;
	}
	default:
		return NONE;
	}

	// Only keep buffer through first \n in this iteration
	bool endFound = false;
	for (uint32_t i = 0; i < bytesRead; i++) {
		if (inputBuffer[i] == '\n') {
			bytesRead = i + 1;
			endFound = true;
			break;
		}
	}
	// If no command end indicator found, stop since there is nothing to parse
	if (!endFound) {
		return NONE;
	}

	// Bytes extracted counts number of bytes minus stripped \r\n from the end (\r might not be there depending on input)
	uint8_t bytesExtracted = 0;
	if (bytesRead >= 2 && inputBuffer[bytesRead - 2] == '\r')
		bytesExtracted = bytesRead - 2;
	else if (bytesRead >= 1 && inputBuffer[bytesRead - 1] == '\n') {
		bytesExtracted = bytesRead - 1;
	}

	// Add null terminator to input buffer to turn into string
	inputBuffer[bytesExtracted] = '\0';

	// Split input buffer by spaces into array to get argc and argv
	// First argument is application name, so must start with argc = 1
	char* argv[MAX_ARGS] = {0};
	argv[0] = "F"; // Fake application name
	int argc = 1;
	char* token = strtok(inputBuffer, " ");
	while (token != NULL) {
		if (argc >= MAX_ARGS) {
			break;
		}
		argv[argc] = token;
		argc++;
		token = strtok(NULL, " ");
	}

	// Parse buffer in loop with getopt
	int opt = 0;
	int optionIndex = 0;
	optind = 0;
	primaryCommand = NONE;
	while ((opt = getopt_long(argc, argv, "d:m:e:t:f:h", longOptions, &optionIndex)) != -1) {
		switch(opt) {
		case 0:
			// New primary command was set
			cliOptionVals.d = NULL;
			cliOptionVals.m = NULL;
			cliOptionVals.f = NULL;
			cliOptionVals.e = NULL;
			cliOptionVals.t = NULL;
			cliOptionVals.h = false;
			break;
		case 'f':
			if (primaryCommand == OFFLOAD) {
				cliOptionVals.f = optarg;
			}
			break;
		case 'h':
			if (primaryCommand == OFFLOAD || primaryCommand == CONFIG) {
				cliOptionVals.h = true;
			}
			break;
		case 'd':
			if (primaryCommand == CONFIG) {
				cliOptionVals.d = optarg;
			}
			break;
		case 'm':
			if (primaryCommand == CONFIG) {
				cliOptionVals.m = optarg;
			}
			break;
		case 'e':
			if (primaryCommand == CONFIG) {
				cliOptionVals.e = optarg;
			}
			break;
		case 't':
			if (primaryCommand == CONFIG) {
				cliOptionVals.t = optarg;
			}
			break;
		default:
			break;
		}
	}

	// Flush input buffer
	switch (commsType) {
	case CLI_BLUETOOTH:
		break;
	case CLI_RADIO:
		break;
	case CLI_USB:
		cbDequeue(HM_UsbGetRxBuffer(), bytesRead);
		break;
	default:
		break;
	}

	// If end-of-command found but primary command doesn't exist, send NACK
	if (primaryCommand == NONE) {
		cliSendAck(false, "Command not recognized");
	}

	// Return primary command entered by user
	return (CliCommand_t) primaryCommand;
}

void cliSend(const char* msg) {
	switch(lastCommsType) {
	case CLI_BLUETOOTH:
		HM_BluetoothSend((uint8_t*) msg, (uint16_t) strlen(msg));
		break;
	case CLI_RADIO:
		// TODO frequency
		HM_RadioSend(RADIO_HW_433, (uint8_t*) msg, (uint16_t) strlen(msg));
		break;
	case CLI_USB:
		HM_UsbTransmit((uint8_t*) msg, (uint16_t) strlen(msg));
		break;
	default:
		break;
	}
}

void cliSendAck(bool success, const char* errMsg) {
	// Upon reception of command, send ERR or OK back to user
	cliSend("\r\n");
	if (success) {
		cliSend("OK\r\n");
	}
	else {
		cliSend("ERR: ");
		if (errMsg)
			cliSend(errMsg);
		cliSend("\r\n\r\n");
	}
}

void cliSendComplete(bool completeSuccess, const char* errMsg) {
	// Upon completion of data sending, send DONE or DONE-ERR back to user
	cliSend("\r\n");
	if (completeSuccess) {
		cliSend("DONE\r\n");
	}
	else {
		cliSend("DONE-ERR: ");
		if (errMsg)
			cliSend(errMsg);
		cliSend("\r\n");
	}
	cliSend("\r\n");
}

CliOptionVals_t cliGetOptions() {
	return cliOptionVals;
}

CircularBuffer_t* cliGetRxBuffer() {
	switch(lastCommsType) {
	case CLI_BLUETOOTH:
		return NULL;
	case CLI_RADIO:
		return NULL;
	case CLI_USB:
		return HM_UsbGetRxBuffer();
	default:
		return NULL;
	}
}
