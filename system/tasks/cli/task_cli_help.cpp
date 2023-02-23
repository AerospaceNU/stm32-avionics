#include <cstdio>

#include "cli.h"
#include "cli_tasks.h"
#include "hardware_manager.h"

static char doc[180 + 5];

/**
 * @brief Generate a single doc line by sending to command line with correct
 * formatting
 * @param primaryCommand: Name of primary command to document
 * @param args: Description of arguments that can be passed in
 * @param desc: Description of behavior
 */
static void generateDoc(const char* primaryCommand, const char* args,
                        const char* desc) {
  snprintf(doc, sizeof(doc), "%-15s %-15s %-150s\r\n", primaryCommand, args,
           desc);
  cli_send(doc);
}

void CliTasks::help() {
  // Send initial ACK to CLI
  cli_sendAck(true, nullptr);

  // Send help docs to user
  cli_send("\r\n");
  generateDoc("--config", "-t int*1",
              "Configure a trigger with additional flag: ");
  generateDoc("", "     -m int*1", "Trigger type (required)");
  generateDoc("", "             ", "Type 1 = pyro");
  generateDoc("", "             ", "Type 2 = line cutter");
  generateDoc("", "             ", "Type 3 = digital on (on pyro)");
  generateDoc("", "             ", "Type 4 = digital off (on pyro)");
  generateDoc("", "             ", "Type 5 = pwm (on pyro)");
  generateDoc("", "     -p int*1", "Pyro num or cut channel (required)");
  generateDoc("", "     -d float*1", "Duration (required for pyro and pwm)");
  generateDoc("", "     -w float*1", "Pulse width (required for pwm)");
  generateDoc("", "     -C string*1",
              "Configuration using expression notation, required");
  generateDoc("", "     -D", "Delete this trigger");
  generateDoc("", "-e float*1", "Configure ground elevation (in meters)");
  generateDoc("", "-r float*1", "Configure ground temperature (in celsius)");
  generateDoc("", "-c int*1",
              "Configure radio channel (in multiples of bandwidth), negative "
              "numbers allowed");
  generateDoc("", "-h", "Help for config. Prints current configuration values");
  generateDoc("--create_flight", "",
              "Clear state log and move back to preflight");
  generateDoc("--erase", "", "Fully erases on-board flash");
  generateDoc("--help", "", "FCB produces standard command line help string");
  generateDoc("--linecutter", "-i int*1",
              "Send linecutter cut with given ID (required)");
  generateDoc("", "-c string*1", "Send a command (required)");
  generateDoc("--offload", "",
              "Offloads the last flight recorded on the board");
  generateDoc("", "-f int*1", "Offload a specific flight number off the board");
  generateDoc("", "-h", "Help for offload. Prints info about each flight");
  generateDoc("--triggerfire", "-t int*1",
              "IMMEDIATELY fires the given trigger number");
  generateDoc("--sense", "", "Reads back most recent sensor data");
  generateDoc("--sim", "", "Simulate past flights in hardware");
  generateDoc("--version", "", "Send Git version and tag info");
  cli_sendComplete(true, nullptr);
}
