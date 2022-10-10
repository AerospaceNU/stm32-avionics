/*
 * cli.h
 */

#ifndef SYSTEM_TASKS_CLI_H_
#define SYSTEM_TASKS_CLI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "board_config_common.h"
#include "circular_buffer.h"
#include "data_structures.h"
#include "trigger_manager.h"

/**
 * Ways the command line can receive information
 */
typedef enum { CLI_PHONE, CLI_RADIO, CLI_USB, NUM_CLI_COMMS } CliComms_e;

/**
 * CLI command types
 */
typedef enum {
  NONE = 0,
  CONFIG,
  CREATE_NEW_FLIGHT,
  ERASE_FLASH,
  OFFLOAD,
  LINECUTTER,
  SENSE,
  SIM,
  HELP,
  TRIGGERFIRE,
  VERSION,
  NUM_CLI_COMMANDS,
} CliCommand_e;

/**
 * Stored argument values
 */
typedef struct {
  char* f;      // flight number
  char* t;      // trigger number
  char* m;      // trigger mode
  char* p;      // device port, e.g. pyro port or line cutter channel
  char* H;      // trigger deploy altitude
  char* D;      // trigger deploy apogee delay
  bool A;       // trigger mode (apogee)
  bool L;       // trigger mode (launch)
  bool T;       // trigger mode (touchdown)
  bool M;       // trigger mode (manual)
  bool C;       // trigger mode (custom marmon clamp)
  char* e;      // ground elevation
  char* r;      // ground temperature
  bool h;       // help flag
  char* c;      // Radio channel, can be negative
  char* lcCmd;  // Line cuttter command
  char* lcId;   // Line cuttter id
} CliOptionVals_s;

/**
 * Configs that can be changed via CLI
 */
typedef struct __attribute__((__packed__)) {
  TriggerConfig_s triggerConfiguration[MAX_TRIGGER];
  double groundElevationM;
  double groundTemperatureC;
  int32_t radioChannel;
} CliConfigs_s;

/**
 * @brief Initializes command line expectations, including arguments, etc.
 */
void cli_init();

/**
 * @brief Return current command line configs
 * @return Pointer to statically-allocated CLI configs
 */
CliConfigs_s* cli_getConfigs();

/**
 * @brief Set CLI config values to defaults
 */
void cli_setDefaultConfig();

/**
 * @brief Parses all bytes that have come in through the specified comms type
 * @param comms_type: Type of comms to get command from
 * @return If there is a complete command detected, returns the primary command
 * entered
 *
 * Note that only the last full command before a \n is processed each loop.
 * This means the user cannot enter two commands in the same line and expect
 * both to execute. This behavior is necessary to know when an option ends since
 * there are no set number of chars.
 */
CliCommand_e cli_parse(CliComms_e commsType);

/**
 * @brief Send message back to user via appropriate medium
 * @param msg: Message to send to user as string
 */
void cli_send(const char* msg);

/**
 * @brief Send acknowledgement of receive back to user
 * @param success: Whether acknowledgement should be a success or failure
 * @param errMsg: Message to send if error occurred
 */
void cli_sendAck(bool success, const char* errMsg);

/**
 * @brief Send task completion status back to user
 * @param completeSuccess: Whether completion was success or failure
 * @param errMsg: Message to send if error occurred
 */
void cli_sendComplete(bool completeSuccess, const char* errMsg);

/**
 * @brief Returns cli options (values and flags) raw (no conversions to other
 * data types)
 * @return Values of CLI options
 */
CliOptionVals_s cli_getOptions();

/**
 * @brief Returns current RX buffer from the appropriate medium
 * @return Pointer to circular buffer
 */
CircularBuffer_s* cli_getRxBuffer();

CircularBuffer_s* cli_getRxBufferFor(CliComms_e source);

#ifdef __cplusplus
}
#endif

#endif  // SYSTEM_TASKS_CLI_H_
