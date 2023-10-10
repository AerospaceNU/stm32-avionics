/*
 * cli.c
 */

#include "cli.h"

#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "board_config_common.h"
#include "hardware_manager.h"
#include "radio_manager.h"

#define INPUT_BUFFER_SIZE 2048
#define MAX_ARGS 200

static char
    inputBuffer[INPUT_BUFFER_SIZE +
                1];  // +1 accounts for null terminator required for strtok

static uint8_t radioRxBuffer[INPUT_BUFFER_SIZE];
static CircularBuffer_s radioRxCircBuffer;

static CliOptionVals_s cliOptionVals = {
    .f = NULL,      // flight number
    .t = NULL,      // trigger number
    .m = NULL,      // trigger mode
    .p = NULL,      // device port, e.g. pyro port or line cutter channel
    .d = NULL,      // trigger duration
    .w = NULL,      // trigger pulse width
    .C = NULL,      // trigger configuration
    .D = false,     // trigger delete
    .N = false,     // trigger not allowed manually
    .e = NULL,      // ground elevation
    .r = NULL,      // ground temperature
    .h = false,     // help flag
    .c = NULL,      // Radio channel, can be negative
    .lcCmd = NULL,  // Line cuttter command
    .lcId = NULL    // Line cuttter id
};

static int primaryCommand = 0;

static struct option longOptions[] = {
    {"config", no_argument, &primaryCommand, CONFIG},
    {"create_flight", no_argument, &primaryCommand, CREATE_NEW_FLIGHT},
    {"erase", no_argument, &primaryCommand, ERASE_FLASH},
    {"help", no_argument, &primaryCommand, HELP},
    {"offload", no_argument, &primaryCommand, OFFLOAD},
    {"sim", no_argument, &primaryCommand, SIM},
    {"sense", no_argument, &primaryCommand, SENSE},
    {"triggerfire", no_argument, &primaryCommand, TRIGGERFIRE},
    {"linecutter", no_argument, &primaryCommand, LINECUTTER},
    {"version", no_argument, &primaryCommand, VERSION},
    {0, 0, 0, 0}};

static CliConfigs_s cliConfigs = {0};

static CliComms_e lastCommsType;  // Used to help send ack to right places
static uint8_t lastStringId = 0xFF;

static void cli_parseRadio(RadioRecievedPacket_s* packet) {
  // Only accept packets with good CRC
  RadioPacket_s* parsedPacket = (RadioPacket_s*)&packet->data;
  if (parsedPacket->packetType == TELEMETRY_ID_STRING) {
    if (packet->crc) {
      if (parsedPacket->payload.cliString.id == lastStringId) {
        // duplicate string, do nothing
        return;
      }
      lastStringId = parsedPacket->payload.cliString.id;
      const uint8_t len = parsedPacket->payload.cliString.len;
      const uint8_t* pdata = parsedPacket->payload.cliString.string;
      for (size_t i = 0; i < len; i++) {
        cb_enqueue(&radioRxCircBuffer, (unknownPtr_t)pdata + i);
      }
    } else {
      cli_sendAck(false, "Bad CRC!");
    }
  }
}

void cli_init() {
  opterr = 0;  // Don't print any messages to standard error stream since this
               // is embedded device

  cb_init(&radioRxCircBuffer, (unknownPtr_t)radioRxBuffer,
          sizeof(radioRxBuffer), 1);
  radioManager_addMessageCallback(RADIO_CLI_ID, cli_parseRadio);
}

CliConfigs_s* cli_getConfigs() { return &cliConfigs; }

void cli_setDefaultConfig() {
  cliConfigs.groundElevationM = 0;
  cliConfigs.groundTemperatureC = 14.85;
  cliConfigs.radioChannel = 1;
  triggerManager_setDefaultConfig();
}

CliCommand_e cli_parse(CliComms_e commsType) {
  // Get buffer from hardware manager
  uint32_t bytesRead = 0;  // Raw bytes read from hardware manager to be
                           // discarded, eventually including \r\n

  // Find the appropriate circular buffer for our medium
  CircularBuffer_s* selectedRxBuffer = cli_getRxBufferFor(commsType);

  if (selectedRxBuffer == NULL) return NONE;
  if (!cb_count(selectedRxBuffer)) return NONE;

  // Read buffer, flush if full (likely bad inputs), and copy to input buffer
  bytesRead = cb_count(selectedRxBuffer);  // Each element 1 byte
  if (cb_full(selectedRxBuffer)) {
    cb_flush(selectedRxBuffer);
    bytesRead = 0;
  }
  cb_peek(selectedRxBuffer, (unknownPtr_t)inputBuffer, NULL);

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

  // Bytes extracted counts number of bytes minus stripped \r\n from the end (\r
  // might not be there depending on input)
  uint32_t bytesExtracted = 0;
  if (bytesRead >= 2 && inputBuffer[bytesRead - 2] == '\r') {
    bytesExtracted = bytesRead - 2;
  } else if (bytesRead >= 1 && inputBuffer[bytesRead - 1] == '\n') {
    bytesExtracted = bytesRead - 1;
  }

  // Add null terminator to input buffer to turn into string
  inputBuffer[bytesExtracted] = '\0';

  // Split input buffer by spaces into array to get argc and argv
  // First argument is application name, so must start with argc = 1
  char* argv[MAX_ARGS] = {0};
  argv[0] = (char*)"F";  // Fake application name
  int argc = 1;

  char* token = strtok(inputBuffer, " ");

  bool isInString = false;  // Keeps track of whether it should delimit by " or
                            // by space. Default is by space

  while (token != NULL && argc < MAX_ARGS) {
    argv[argc] = token;
    argc++;

    size_t tokenLength = strnlen(token, (size_t)(inputBuffer - token));

    if (!isInString) {
      if (*(token + tokenLength + 1) == '\"')
        isInString =
            true;  // If next token starts with a ", set isInString to true
    } else {
      if (*(token + tokenLength - 1) == '\"')
        isInString =
            false;  // If current token ends with a ", set isInString to false
    }

    if (isInString) {
      token = strtok(token + tokenLength + 2,
                     "\"");  // Delimits tokens by " instead of by space, skips
                             // past open quote
    } else {
      token = strtok(NULL, " ");
    }
  }

  bool invalidOptCommand = false;
  // Parse buffer in loop with getopt
  int opt = 0;
  int optionIndex = 0;
  optind = 0;
  primaryCommand = NONE;
  while ((opt = getopt_long(argc, argv, "f:t:m:p:d:w:C:e:r:c:i:DhN",
                            longOptions, &optionIndex)) != -1) {
    switch (opt) {
      case 0:
        // New primary command was set
        cliOptionVals.f = NULL;
        cliOptionVals.t = NULL;
        cliOptionVals.m = NULL;
        cliOptionVals.p = NULL;
        cliOptionVals.d = NULL;
        cliOptionVals.w = NULL;
        cliOptionVals.C = NULL;
        cliOptionVals.D = false;
        cliOptionVals.N = false;
        cliOptionVals.e = NULL;
        cliOptionVals.r = NULL;
        cliOptionVals.h = false;
        cliOptionVals.c = NULL;
        cliOptionVals.lcCmd = NULL;
        cliOptionVals.lcId = NULL;
        break;
      case 'f':
        if (primaryCommand == OFFLOAD) {
          cliOptionVals.f = optarg;
        } else {
          invalidOptCommand = true;
        }
        break;
      case 't':
        if (primaryCommand == CONFIG || primaryCommand == TRIGGERFIRE) {
          cliOptionVals.t = optarg;
        } else {
          invalidOptCommand = true;
        }
        break;
      case 'm':
        if (primaryCommand == CONFIG) {
          cliOptionVals.m = optarg;
        } else {
          invalidOptCommand = true;
        }
        break;
      case 'p':
        if (primaryCommand == CONFIG) {
          cliOptionVals.p = optarg;
        } else {
          invalidOptCommand = true;
        }
        break;
      case 'd':
        if (primaryCommand == CONFIG) {
          cliOptionVals.d = optarg;
        } else {
          invalidOptCommand = true;
        }
        break;
      case 'w':
        if (primaryCommand == CONFIG) {
          cliOptionVals.w = optarg;
        } else {
          invalidOptCommand = true;
        }
        break;
      case 'C':
        if (primaryCommand == CONFIG) {
          cliOptionVals.C = optarg;
        } else {
          invalidOptCommand = true;
        }
        break;
      case 'D':
        if (primaryCommand == CONFIG) {
          cliOptionVals.D = true;
        } else {
          invalidOptCommand = true;
        }
        break;
      case 'N':
        if (primaryCommand == CONFIG) {
          cliOptionVals.N = true;
        } else {
          invalidOptCommand = true;
        }
        break;
      case 'h':
        if (primaryCommand == OFFLOAD || primaryCommand == CONFIG) {
          cliOptionVals.h = true;
        } else {
          invalidOptCommand = true;
        }
        break;
      case 'e':
        if (primaryCommand == CONFIG) {
          cliOptionVals.e = optarg;
        } else {
          invalidOptCommand = true;
        }
        break;
      case 'r':
        if (primaryCommand == CONFIG) {
          cliOptionVals.r = optarg;
        } else {
          invalidOptCommand = true;
        }
        break;
      case 'c':
        // Radio channel
        if (primaryCommand == CONFIG) {
          cliOptionVals.c = optarg;
        } else if (primaryCommand == LINECUTTER) {
          cliOptionVals.lcCmd = optarg;
        } else {
          invalidOptCommand = true;
        }
        break;
      // Line cutter ID
      case 'i':
        if (primaryCommand == LINECUTTER) {
          cliOptionVals.lcId = optarg;
        } else {
          invalidOptCommand = true;
        }
        break;

      default:
        break;
    }

    if (invalidOptCommand) {
      int i = primaryCommand - 1;
      if (i < 0) break;
      const char* invalidCommand = longOptions[i].name;
      char errMsg[128];
      snprintf(errMsg, sizeof errMsg,
               "Invalid option: \"-%c\" is not a valid option for %s", opt,
               invalidCommand);
      cli_sendAck(false, errMsg);
    }
  }

  // Set last comms type (at the end, so we only change to a comm type that gave
  // us a command)
  lastCommsType = commsType;

  // Flush input buffer
  cb_dequeue(selectedRxBuffer, bytesRead);

  // If end-of-command found but primary command doesn't exist, send NACK
  if (primaryCommand == NONE) {
    cli_sendAck(false, "Command not recognized");
  }
  if (invalidOptCommand) primaryCommand = NONE;

  // Return primary command entered by user
  return (CliCommand_e)primaryCommand;
}

void cli_send(const char* msg) {
  switch (lastCommsType) {
    case CLI_PHONE:
      hm_bleClientSend(BLE_CLI_ID, (uint8_t*)msg, (uint16_t)strlen(msg));
      break;
    case CLI_RADIO:
      radioManager_transmitString(RADIO_CLI_ID, (uint8_t*)msg, strlen(msg));
      break;
    case CLI_USB:
      hm_usbTransmit(USB_CLI_ID, (uint8_t*)msg, (uint16_t)strlen(msg));
      break;
    default:
      break;
  }
}

void cli_sendAck(bool success, const char* errMsg) {
  // Upon reception of command, send ERR or OK back to user
  cli_send("\r\n");
  if (success) {
    cli_send("OK\r\n");
  } else {
    cli_send("ERR: ");
    if (errMsg) cli_send(errMsg);
    cli_send("\r\n\r\n");
  }
}

void cli_sendComplete(bool completeSuccess, const char* errMsg) {
  // Upon completion of data sending, send DONE or DONE-ERR back to user
  cli_send("\r\n");
  if (completeSuccess) {
    cli_send("DONE\r\n");
  } else {
    cli_send("DONE-ERR: ");
    if (errMsg) cli_send(errMsg);
    cli_send("\r\n");
  }
  cli_send("\r\n");
}

CliOptionVals_s cli_getOptions() { return cliOptionVals; }

CircularBuffer_s* cli_getRxBufferFor(CliComms_e source) {
  switch (source) {
    case CLI_PHONE:
      return hm_bleClientGetRxBuffer(BLE_CLI_ID);
    case CLI_RADIO:
      return &radioRxCircBuffer;
    case CLI_USB:
      return hm_usbGetRxBuffer(USB_CLI_ID);
    default:
      return NULL;
  }
}

CircularBuffer_s* cli_getRxBuffer() {
  return cli_getRxBufferFor(lastCommsType);
}
