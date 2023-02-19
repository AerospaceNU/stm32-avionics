#include "state_cli_offload.h"

#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#include "buzzer_heartbeat.h"
#include "cli.h"
#include "data_log.h"  // This include is only needed to pass a flightID to userSetFlightID before command line implementation
#include "data_offload.h"

static void formatTimeSeconds(char *result, int resultLen, uint32_t seconds) {
  uint32_t minutes = seconds / 60;
  seconds %= 60;

  uint32_t hours = minutes / 60;
  minutes %= 60;

  snprintf(result, resultLen, "%02" PRIu32 ":%02" PRIu32 ":%02" PRIu32, hours,
           minutes, seconds);
}

void CliOffloadState::init() {
  CliOptionVals_s cliOptions = cli_getOptions();

  // If specific flight number is provided, try offloading that
  if (cliOptions.f) {
    uint32_t flightNum = (uint32_t)atoi(cliOptions.f);
    // Flight number 0 explicitly doesn't exist, so 0 means atoi failed
    if (flightNum == 0) {
      initSuccess_ = false;
      cli_sendAck(false, "Flight number invalid integer or 0");
      return;
    }
    if (flightNum > dataLog_getLastFlightNum()) {
      initSuccess_ = false;
      cli_sendAck(false, "Flight number doesn't exist");
      return;
    }
    dataOffload_setFlightId(flightNum);
  } else {
    // Otherwise, offload most recent launched flight
    uint32_t lastLaunched = dataLog_getLastLaunchedFlightNum();
    // Offload the last launched flight if it exists, or else the last flight if
    // none are launched
    dataOffload_setFlightId(lastLaunched > 0 ? lastLaunched
                                             : dataLog_getLastFlightNum());
  }

  // Send ack to command line if reached
  initSuccess_ = true;
  cli_sendAck(true, nullptr);
}

EndCondition_e CliOffloadState::run() {
  // If initialization didn't succeed, stop
  if (!initSuccess_) {
    return CliCommandComplete;
  }

  // Run buzzer heartbeat
  buzzerHeartbeat_tick();

  // If help command, send help and stop
  if (cli_getOptions().h) {
    char sendString[80];
    uint32_t lastFlightNum = dataLog_getLastFlightNum();
    if (lastFlightNum == 0) {
      snprintf(sendString, sizeof(sendString),
               "\r\nNo flights exist to offload\r\n");
      cli_send(sendString);
    } else {
      time_t flight_timestamp;
      char timeString[25];
      snprintf(sendString, sizeof(sendString),
               "Available flights to offload: \r\n");
      cli_send(sendString);
      snprintf(sendString, sizeof(sendString),
               "Last: %" PRIu32 "      Last Launched: %" PRIu32 "\r\n",
               dataLog_getLastFlightNum(), dataLog_getLastLaunchedFlightNum());
      cli_send(sendString);
      snprintf(sendString, sizeof(sendString),
               "| %8s | %8s | %-25s | %-15s |\r\n", "Flight #", "Launched",
               "Timestamp", "Flight Duration");
      cli_send(sendString);
      for (uint8_t num = 1; num <= lastFlightNum; ++num) {
        dataLog_readFlightNumMetadata(num);
        FlightMetadata_s *metadataPacket = dataLog_getFlightMetadata();
        flight_timestamp = (time_t)metadataPacket->gpsTimestamp;
        // Check timestamp between 2000 and 2100
        if (flight_timestamp > 946702800 && flight_timestamp < 4102462800) {
          snprintf(timeString, sizeof(timeString), "%" PRIu64,
                   flight_timestamp);
        } else {
          snprintf(timeString, sizeof(timeString), "None");
        }

        char launchedString[6];
        snprintf(launchedString, sizeof(launchedString),
                 metadataPacket->launched == 1 ? "true" : "false");
        char durationString[9];
        formatTimeSeconds(durationString, sizeof(durationString),
                          dataLog_getLastFlightTimestamp(num) / 1000);
        snprintf(sendString, sizeof(sendString),
                 "| %8d | %8s | %-25s | %-15s |\r\n", num, launchedString,
                 timeString, durationString);
        cli_send(sendString);
      }
    }
    return EndCondition_e::CliCommandComplete;
  }

  // Offload one chunk of data
  if (dataOffload_tick()) {
    return EndCondition_e::CliCommandComplete;
  }

  return EndCondition_e::NoChange;
}

void CliOffloadState::cleanup() {
  // Only send complete message if initialization was successful
  if (initSuccess_) {
    cli_sendComplete(true, nullptr);
  }
}
