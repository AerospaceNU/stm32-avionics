/*
 * cli.c
 */

#include "cli.h"

#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "board_config_common.h"
#include "hardware_manager.h"
#include "radio_manager.h"

#define INPUT_BUFFER_SIZE 2048
#define MAX_ARGS 50

static char
    inputBuffer[INPUT_BUFFER_SIZE +
                1];  // +1 accounts for null terminator required for strtok

static uint8_t radioRxBuffer[INPUT_BUFFER_SIZE];
static CircularBuffer_s radioRxCircBuffer;

static CliOptionVals_s cliOptionVals = {.f = NULL,
                                        .t = NULL,
                                        .m = NULL,
                                        .p = NULL,
                                        .H = NULL,
                                        .D = NULL,
                                        .e = NULL,
                                        .r = NULL,
                                        .c = NULL,
                                        .A = false,
                                        .L = false,
                                        .T = false,
                                        .M = false,
                                        .C = false,
                                        .h = false,
                                        .lcCmd = NULL,
                                        .lcId = NULL};

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

static void cli_parseRadio(RadioRecievedPacket_s* packet) {
  // Only accept packets with good CRC
  RadioPacket_s* parsedPacket = (RadioPacket_s*)&packet->data;
  if (parsedPacket->packetType == TELEMETRY_ID_STRING) {
    if (packet->crc) {
      const uint8_t len = parsedPacket->payload.cliString.len;
      const uint8_t* pdata = parsedPacket->payload.cliString.string;
      for (size_t i = 0; i < len; i++) {
        cb_enqueue(&radioRxCircBuffer, pdata + i);
      }
    } else {
      cli_sendAck(false, "Bad CRC!");
    }
  }
}

void cli_init() {
  opterr = 0;  // Don't print any messages to standard error stream since this
               // is embedded device

  cb_init(&radioRxCircBuffer, radioRxBuffer, sizeof(radioRxBuffer), 1);
  radioManager_addMessageCallback(RADIO_CLI_ID, cli_parseRadio);
}

CliConfigs_s* cli_getConfigs() { return &cliConfigs; }

void cli_setDefaultConfig() {
#if NUM_PYRO > 0
  cliConfigs.triggerConfiguration[0].mode = TRIGGER_TYPE_PYRO;
  cliConfigs.triggerConfiguration[0].port = 0;  // Pyro # 1
  cliConfigs.triggerConfiguration[0].flags = FLAG_APOGEE;
#if NUM_PYRO > 1
  cliConfigs.triggerConfiguration[0].mode = TRIGGER_TYPE_PYRO;
  cliConfigs.triggerConfiguration[0].port = 1;  // Pyro # 2
  cliConfigs.triggerConfiguration[1].flags = FLAG_ALT_DURING_DESCENT;
  cliConfigs.triggerConfiguration[1].configValue = 230;  // 230 m deploy

#endif  // NUM_PYRO > 1
#endif  // NUM_PYRO > 0
  cliConfigs.groundElevationM = 0;
  cliConfigs.groundTemperatureC = 14.85;
  cliConfigs.radioChannel = 1;
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
  cb_peek(selectedRxBuffer, inputBuffer, NULL);

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
  uint8_t bytesExtracted = 0;
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
  argv[0] = "F";  // Fake application name
  int argc = 1;

  char* token = strtok(inputBuffer, " ");

  bool isInString = false;  // Keeps track of whether it should delimit by " or
                            // by space. Default is by space

  while (token != NULL && argc < MAX_ARGS) {
    argv[argc] = token;
    argc++;

    int tokenLength = strnlen(token, inputBuffer - token);

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

  // Parse buffer in loop with getopt
  int opt = 0;
  int optionIndex = 0;
  optind = 0;
  primaryCommand = NONE;
  while ((opt = getopt_long(argc, argv, "m:r:p:H:D:e:t:f:c:i:ALTMhC",
                            longOptions, &optionIndex)) != -1) {
    switch (opt) {
      case 0:
        // New primary command was set
        cliOptionVals.f = NULL;
        cliOptionVals.t = NULL;
        cliOptionVals.m = NULL;
        cliOptionVals.p = NULL;
        cliOptionVals.H = NULL;
        cliOptionVals.D = NULL;
        cliOptionVals.e = NULL;
        cliOptionVals.r = NULL;
        cliOptionVals.c = NULL;
        cliOptionVals.A = false;
        cliOptionVals.L = false;
        cliOptionVals.T = false;
        cliOptionVals.M = false;
        cliOptionVals.C = false;
        cliOptionVals.h = false;
        cliOptionVals.lcCmd = NULL;
        cliOptionVals.lcId = NULL;
        break;
      case 'f':
        if (primaryCommand == OFFLOAD) {
          cliOptionVals.f = optarg;
        }
        break;
      case 't':
        if (primaryCommand == CONFIG || primaryCommand == TRIGGERFIRE) {
          cliOptionVals.t = optarg;
        }
        break;
      case 'm':
        if (primaryCommand == CONFIG) {
          cliOptionVals.m = optarg;
        }
        break;
      case 'p':
        if (primaryCommand == CONFIG) {
          cliOptionVals.p = optarg;
        }
        break;
      case 'A':
        if (primaryCommand == CONFIG) {
          cliOptionVals.A = true;
        }
        break;
      case 'L':
        if (primaryCommand == CONFIG) {
          cliOptionVals.L = true;
        }
        break;
      case 'T':
        if (primaryCommand == CONFIG) {
          cliOptionVals.T = true;
        }
        break;
      case 'M':
        if (primaryCommand == CONFIG) {
          cliOptionVals.M = true;
        }
        break;
      case 'C':
        if (primaryCommand == CONFIG) {
          cliOptionVals.C = true;
        }
        break;
      case 'H':
        if (primaryCommand == CONFIG) {
          cliOptionVals.H = optarg;
        }
        break;
      case 'D':
        if (primaryCommand == CONFIG) {
          cliOptionVals.D = optarg;
        }
        break;
      case 'h':
        if (primaryCommand == OFFLOAD || primaryCommand == CONFIG) {
          cliOptionVals.h = true;
        }
        break;

      case 'e':
        if (primaryCommand == CONFIG) {
          cliOptionVals.e = optarg;
        }
        break;
      case 'r':
        if (primaryCommand == CONFIG) {
          cliOptionVals.r = optarg;
        }
        break;
      case 'c':
        // Radio channel
        if (primaryCommand == CONFIG) {
          cliOptionVals.c = optarg;
        } else if (primaryCommand == LINECUTTER) {
          cliOptionVals.lcCmd = optarg;
        }
        break;

      // Line cutter ID
      case 'i':
        if (primaryCommand == LINECUTTER) {
          cliOptionVals.lcId = optarg;
        }
        break;

      default:
        break;
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
