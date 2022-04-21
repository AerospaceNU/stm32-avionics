#include <stdio.h>

#include "cli.h"
#include "cli_tasks.h"
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
  char doc[177 + 5];
  snprintf(doc, sizeof(doc), "%-12s %-15s %-150s\r\n", primaryCommand, args,
           desc);
  cliSend(doc);
}

void cli_tasks::cliHelp() {
  // Send initial ACK to CLI
  cliSendAck(true, nullptr);

  // Send help docs to user
  cliSend("\r\n");
  generateDoc("--calibrate", "TBD",
              "Calibration routine is still undefined. Arguments to the "
              "command will be guided by that routine");
  generateDoc("--config", "-p int*1",
              "Configure a pyro with additional flag: ");
  generateDoc("", "     -A", "Deploy at apogee");
  generateDoc("", "     -D float*1", "Deploy n seconds after apogee");
  generateDoc("", "     -H float*1", "Deploy at given descent altitude");
  generateDoc("", "-e float*1", "Configure ground elevation (in meters)");
  generateDoc("", "-t float*1", "Configure ground temperature (in celsius)");
  generateDoc("", "-c int*1",
              "Configure radio channel (in multiples of bandwidth), negative "
              "numbers allowed");
  generateDoc("", "-h", "Help for config. Prints current configuration values");
  generateDoc("--erase", "", "Fully erases on-board flash");
  generateDoc("--help", "", "FCB produces standard command line help string");
  generateDoc("--pyrofire", "-p int*1",
              "IMMEDIATELY fires the given pyro number");
  generateDoc("--offload", "",
              "Offloads the last flight recorded on the board");
  generateDoc("", "-f int*1", "Offload a specific flight number off the board");
  generateDoc("", "-h", "Help for offload. Prints info about each flight");
  generateDoc("--sense", "", "Reads back most recent sensor data");
  generateDoc("--sim", "", "Simulate past flights in hardware");
  generateDoc("--create_flight", "", "Clear state log and move back to preflight");
  cliSendComplete(true, nullptr);
}
