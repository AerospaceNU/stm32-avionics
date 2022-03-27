
#include "state_cli_help.h"

#include <stdio.h>

#include "cli.h"
#include "hardware_manager.h"

/**
 * @brief Generate a single doc line by sending to command line with correct
 * formatting
 * @param primaryCommand: Name of primary command to document
 * @param args: Description of arguments that can be passed in
 * @param desc: Description of behavior
 */
static void generateDoc(const char* primaryCommand, const char* args,
                        const char* desc) {
  char doc[173 + 5];
  snprintf(doc, sizeof(doc), "%-12s %-11s %-150s\r\n", primaryCommand, args,
           desc);
  cliSend(doc);
}

void CliHelpState::init() {
  // Send initial ACK to CLI
  cliSendAck(true, nullptr);
}

EndCondition_t CliHelpState::run() {
  // Send help docs to user
  cliSend("\r\n");
  generateDoc("--calibrate", "TBD",
              "Calibration routine is still undefined. Arguments to the "
              "command will be guided by that routine");
  generateDoc("--config", "-d float*1",
              "Configure another drogue cut event's altitude. FCB will re-sort "
              "the altitudes automatically");
  generateDoc("", "-e float*1", "Configure ground elevation (in meters)");
  generateDoc("", "-t float*1", "Configure ground temperature (in celsius)");
  generateDoc("", "-m float*1", "Configure main cut altitude (in meters)");
  generateDoc("", "-c int*1",
              "Configure radio channel (in multiples of bandwidth), negative "
              "numbers allowed");
  generateDoc("", "-h", "Help for config. Prints current configuration values");
  generateDoc("--erase", "", "Fully erases on-board flash");
  generateDoc("--help", "", "FCB produces standard command line help string");
  generateDoc("--offload", "",
              "Offloads the last flight recorded on the board");
  generateDoc("", "-f int*1", "Offload a specific flight number off the board");
  generateDoc("", "-h", "Help for offload. Prints info about each flight");
  generateDoc("--sense", "", "Reads back most recent sensor data");
  generateDoc("--sim", "", "Simulate past flights in hardware");
  generateDoc("--shutdown", "",
              "Prevent FCB from doing anything else. FCB won't actually shut "
              "off, but it won't do or respond to anything");
  return EndCondition_t::CliCommandComplete;
}

void CliHelpState::cleanup() {
  // Send complete message to CLI
  cliSendComplete(true, nullptr);
}
