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

static void format_time_seconds(char *result, int resultLen, uint32_t seconds) {
  uint32_t minutes = seconds / 60;
  seconds %= 60;

  uint32_t hours = minutes / 60;
  minutes %= 60;

  snprintf(result, resultLen, "%02" PRIu32 ":%02" PRIu32 ":%02" PRIu32, hours,
           minutes, seconds);
}

void CliOffloadState::init() {
  CliOptionVals_s cliOptions = cliGetOptions();

  // If specific flight number is provided, try offloading that
  if (cliOptions.f) {
    uint32_t flightNum = (uint32_t)atoi(cliOptions.f);
    // Flight number 0 explicitly doesn't exist, so 0 means atoi failed
    if (flightNum == 0) {
      initSuccess_ = false;
      cliSendAck(false, "Flight number invalid integer or 0");
      return;
    }
    if (flightNum > data_log_get_last_flight_num()) {
      initSuccess_ = false;
      cliSendAck(false, "Flight number doesn't exist");
      return;
    }
    dataOffloadSetFlightId(flightNum);
  } else {
    // Otherwise, offload most recent launched flight
    uint32_t lastLaunched = data_log_get_last_launched_flight_num();
    // Offload the last launched flight if it exists, or else the last flight if
    // none are launched
    dataOffloadSetFlightId(lastLaunched > 0 ? lastLaunched
                                            : data_log_get_last_flight_num());
  }

  // Send ack to command line if reached
  initSuccess_ = true;
  cliSendAck(true, nullptr);
}

EndCondition_e CliOffloadState::run() {
  // If initialization didn't succeed, stop
  if (!initSuccess_) {
    return CliCommandComplete;
  }

  buzzerHeartbeat();

  // If help command, send help and stop
  if (cliGetOptions().h) {
    char sendString[80];
    uint32_t lastFlightNum = data_log_get_last_flight_num();
    if (lastFlightNum == 0) {
      snprintf(sendString, sizeof(sendString),
               "\r\nNo flights exist to offload\r\n");
      cliSend(sendString);
    } else {
      time_t flight_timestamp;
      char timeString[25];
      snprintf(sendString, sizeof(sendString),
               "Available flights to offload: \r\n");
      cliSend(sendString);
      snprintf(sendString, sizeof(sendString),
               "Last: %" PRIu32 "      Last Launched: %" PRIu32 "\r\n",
               data_log_get_last_flight_num(),
               data_log_get_last_launched_flight_num());
      cliSend(sendString);
      snprintf(sendString, sizeof(sendString),
               "| %8s | %8s | %-25s | %-15s |\r\n", "Flight #", "Launched",
               "Timestamp", "Flight Duration");
      cliSend(sendString);
      for (uint8_t num = 1; num <= lastFlightNum; ++num) {
        data_log_read_flight_num_metadata(num);
        FlightMetadata_s *metadataPacket = data_log_get_flight_metadata();
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
        format_time_seconds(durationString, sizeof(durationString),
                            data_log_get_last_flight_timestamp(num) / 1000);
        snprintf(sendString, sizeof(sendString),
                 "| %8d | %8s | %-25s | %-15s |\r\n", num, launchedString,
                 timeString, durationString);
        cliSend(sendString);
      }
    }
    return EndCondition_e::CliCommandComplete;
  }

  // Offload one chunk of data
  if (dataOffload()) {
    return EndCondition_e::CliCommandComplete;
  }

  return EndCondition_e::NoChange;
}

void CliOffloadState::cleanup() {
  // Only send complete message if initialization was successful
  if (initSuccess_) {
    cliSendComplete(true, nullptr);
  }
}
