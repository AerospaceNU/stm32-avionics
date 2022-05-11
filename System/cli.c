/*
 * cli.c
 */

#include "cli.h"

#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "hardware_manager.h"
#include "radio_manager.h"

#define INPUT_BUFFER_SIZE 2048
#define MAX_ARGS 50

static char
    inputBuffer[INPUT_BUFFER_SIZE +
                1];  // +1 accounts for null terminator required for strtok

static uint8_t radioRxBuffer[INPUT_BUFFER_SIZE];
static CircularBuffer_t radioRxCircBuffer;

static CliOptionVals_t cliOptionVals = {.p = NULL,
                                        .H = NULL,
                                        .D = NULL,
                                        .e = NULL,
                                        .t = NULL,
                                        .f = NULL,
                                        .A = false,
                                        .h = false,
                                        .lcCmd = NULL,
                                        .lcId = NULL};

static int primaryCommand = 0;

static struct option longOptions[] = {
    {"calibrate", no_argument, &primaryCommand, CALIBRATE},
    {"config", no_argument, &primaryCommand, CONFIG},
    {"create_flight", no_argument, &primaryCommand, CREATE_NEW_FLIGHT},
    {"erase", no_argument, &primaryCommand, ERASE_FLASH},
    {"help", no_argument, &primaryCommand, HELP},
    {"offload", no_argument, &primaryCommand, OFFLOAD},
    {"sim", no_argument, &primaryCommand, SIM},
    {"sense", no_argument, &primaryCommand, SENSE},
    {"pyrofire", no_argument, &primaryCommand, PYROFIRE},
    {"linecutter", no_argument, &primaryCommand, LINECUTTER},
    {0, 0, 0, 0}};

static CliConfigs_t cliConfigs;

static CliComms_t lastCommsType;  // Used to help send ack to right places

static void cliParseRadio(RecievedPacket_t* packet) {
  // Only accept packets with good CRC
  RadioPacket_t* parsedPacket = (RadioPacket_t*)&packet->data;
  if (parsedPacket->packetType == TELEMETRY_ID_STRING) {
    if (packet->crc) {
      const uint8_t len = parsedPacket->payload.cliString.len;
      const uint8_t* pdata = parsedPacket->payload.cliString.string;
      for (size_t i = 0; i < len; i++) {
        cbEnqueue(&radioRxCircBuffer, pdata + i);
      }
    } else {
      cliSendAck(false, "Bad CRC!");
    }
  }
}

void cliInit() {
  opterr = 0;  // Don't print any messages to standard error stream since this
               // is embedded device

  cbInit(&radioRxCircBuffer, radioRxBuffer, sizeof(radioRxBuffer), 1);
  RadioManager_addMessageCallback(cliParseRadio);
}

CliConfigs_t* cliGetConfigs() { return &cliConfigs; }

void cliSetDefaultConfig() {
#if MAX_PYRO >= 2
  cliConfigs.pyroConfiguration[0].flags = 0b1;   // FLAG_APOGEE
  cliConfigs.pyroConfiguration[1].flags = 0b10;  // FLAG_DESCENT_ALT_THRESHOLD
  cliConfigs.pyroConfiguration[1].configValue = 230;  // 230 m deploy
#endif
  cliConfigs.groundElevationM = 0;
  cliConfigs.groundTemperatureC = 14.85;
  cliConfigs.radioChannel = 1;
}

CliCommand_t cliParse(CliComms_t commsType) {
  // Get buffer from hardware manager
  uint32_t bytesRead = 0;  // Raw bytes read from hardware manager to be
                           // discarded, eventually including \r\n

  // Find the appropriate circular buffer for our medium
  CircularBuffer_t* selectedRxBuffer = cliGetRxBufferFor(commsType);

  if (selectedRxBuffer == NULL) return NONE;
  if (!cbCount(selectedRxBuffer)) return NONE;

  // Read buffer, flush if full (likely bad inputs), and copy to input buffer
  bytesRead = cbCount(selectedRxBuffer);  // Each element 1 byte
  if (cbFull(selectedRxBuffer)) {
    cbFlush(selectedRxBuffer);
    bytesRead = 0;
  }
  cbPeek(selectedRxBuffer, inputBuffer, NULL);

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
  while ((opt = getopt_long(argc, argv, "p:H:D:e:t:f:c:i:Ah", longOptions,
                            &optionIndex)) != -1) {
    switch (opt) {
      case 0:
        // New primary command was set
        cliOptionVals.p = NULL;
        cliOptionVals.H = NULL;
        cliOptionVals.A = false;
        cliOptionVals.f = NULL;
        cliOptionVals.e = NULL;
        cliOptionVals.t = NULL;
        cliOptionVals.h = false;
        cliOptionVals.c = NULL;
        cliOptionVals.lcCmd = NULL;
        cliOptionVals.lcId = NULL;
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
      case 'p':
        if (primaryCommand == CONFIG || primaryCommand == PYROFIRE) {
          cliOptionVals.p = optarg;
        }
        break;
      case 'A':
        if (primaryCommand == CONFIG) {
          cliOptionVals.A = true;
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
  cbDequeue(selectedRxBuffer, bytesRead);

  // If end-of-command found but primary command doesn't exist, send NACK
  if (primaryCommand == NONE) {
    cliSendAck(false, "Command not recognized");
  }

  // Return primary command entered by user
  return (CliCommand_t)primaryCommand;
}

void cliSend(const char* msg) {
  switch (lastCommsType) {
    case CLI_PHONE:
      HM_BluetoothSend(ADDR_PHONE, (uint8_t*)msg, (uint16_t)strlen(msg));
      break;
    case CLI_RADIO:
#ifdef TELEMETRY_RADIO
      RadioManager_transmitString(TELEMETRY_RADIO, (uint8_t*)msg, strlen(msg));
#endif
      break;
    case CLI_USB:
      HM_UsbTransmit((uint8_t*)msg, (uint16_t)strlen(msg));
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
  } else {
    cliSend("ERR: ");
    if (errMsg) cliSend(errMsg);
    cliSend("\r\n\r\n");
  }
}

void cliSendComplete(bool completeSuccess, const char* errMsg) {
  // Upon completion of data sending, send DONE or DONE-ERR back to user
  cliSend("\r\n");
  if (completeSuccess) {
    cliSend("DONE\r\n");
  } else {
    cliSend("DONE-ERR: ");
    if (errMsg) cliSend(errMsg);
    cliSend("\r\n");
  }
  cliSend("\r\n");
}

CliOptionVals_t cliGetOptions() { return cliOptionVals; }

CircularBuffer_t* cliGetRxBufferFor(CliComms_t source) {
  switch (source) {
    case CLI_PHONE:
      return NULL;
    case CLI_RADIO:
      return &radioRxCircBuffer;
    case CLI_USB:
      return HM_UsbGetRxBuffer();
    default:
      return NULL;
  }
}

CircularBuffer_t* cliGetRxBuffer() { return cliGetRxBufferFor(lastCommsType); }
