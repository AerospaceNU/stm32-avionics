/*
 * cli.h
 */

#ifndef CLI_H_
#define CLI_H_

#ifdef __cplusplus
extern "C"{
#endif

#include <stdbool.h>

/**
 * Ways the command line can receive information
 */
typedef enum CliComms_t {
	CLI_BLUETOOTH,
	CLI_RADIO,
	CLI_USB,
	NUM_CLI_COMMS
} CliComms_t;

/**
 * CLI command types
 */
typedef enum CliCommand_t {
	NONE = 0,
	OFFLOAD,
	CALIBRATE,
	SHUTDOWN,
	CONFIG,
	ERASE_FLASH,
	HELP
} CliCommand_t;

/**
 * Stored argument values
 */
typedef struct {
	char* f; // flight number
	char* d; // drogue altitude
	char* m; // main altitude
	char* e; // ground elevation
	char* t; // ground temperature
	bool h;  // help flag
} CliOptionVals_t;

/**
 * @brief Initializes command line expectations, including arguments, etc.
 */
void cliInit();

/**
 * @brief Parses all bytes that have come in through the specified comms type
 * @param comms_type: Type of comms to get command from
 * @return If there is a complete command detected, returns the primary command entered
 *
 * Note that only the last full command before a \n is processed each loop.
 * This means the user cannot enter two commands in the same line and expect both to execute.
 * This behavior is necessary to know when an option ends since there are no set number of chars.
 */
CliCommand_t cliParse(CliComms_t commsType);

/**
 * @brief Send message back to user via appropriate medium
 * @param msg: Message to send to user as string
 */
void cliSend(const char* msg);

/**
 * @brief Send acknowledgement of receive back to user
 * @param success: Whether acknowledgement should be a success or failure
 * @param errMsg: Message to send if error occurred
 */
void cliSendAck(bool success, const char* errMsg);

/**
 * @brief Send task completion status back to user
 * @param completeSuccess: Whether completion was success or failure
 * @param errMsg: Message to send if error occurred
 */
void cliSendComplete(bool completeSuccess, const char* errMsg);

/**
 * @brief Returns cli options (values and flags) raw (no conversions to other data types)
 * @return Values of CLI options
 */
CliOptionVals_t cliGetOptions();

#ifdef __cplusplus
}
#endif

#endif /* CLI_H_ */
