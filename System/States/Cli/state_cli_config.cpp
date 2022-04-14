
#include "state_cli_config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "board_config.h"
#include "cli.h"
#include "data_log.h"
#include "errno.h"
#include "hardware_manager.h"

static void generateConfigHelp(const char* name, const char* value) {
  char msg[70 + 4];
  snprintf(msg, sizeof(msg), "%-30s %-40s\r\n", name, value);
  cliSend(msg);
}

void CliConfigState::init() {
  // No action
}

EndCondition_t CliConfigState::run() {
  // Get command line options
  CliOptionVals_t options = cliGetOptions();

  // Configure pyro
  if (options.p) {
    char* endPtr;
    int pyroNum = strtol(options.p, &endPtr, 10) - 1;
    if (*endPtr != '\0' || pyroNum < 0 || pyroNum >= MAX_PYRO) {
      cliSendAck(false, "Invalid pyro number");
      return EndCondition_t::CliCommandComplete;
    }
    if (options.A) {
      (cliGetConfigs()->pyroConfiguration + pyroNum)->flags = FLAG_APOGEE;
      // Write new cli configs to flash
      data_log_write_cli_configs();
    } else if (options.H) {
      double pyroAlt = strtod(options.H, &endPtr);
      if (*endPtr != '\0') {
        cliSendAck(false, "Pyro deploy altitude invalid float");
        return EndCondition_t::CliCommandComplete;
      }
      (cliGetConfigs()->pyroConfiguration + pyroNum)->flags =
          FLAG_ALT_DURING_DESCENT;
      (cliGetConfigs()->pyroConfiguration + pyroNum)->configValue = pyroAlt;
      // Write new cli configs to flash
      data_log_write_cli_configs();
    } else if (options.D) {
      double apogeeDelay = strtod(options.D, &endPtr);
      if (*endPtr != '\0') {
        cliSendAck(false, "Apogee delay invalid float");
        return EndCondition_t::CliCommandComplete;
      }
      (cliGetConfigs()->pyroConfiguration + pyroNum)->flags = FLAG_APOGEE_DELAY;
      (cliGetConfigs()->pyroConfiguration + pyroNum)->configValue = apogeeDelay;
      // Write new cli configs to flash
      data_log_write_cli_configs();
    } else {
      cliSendAck(false,
                 "Pyro config must either specify -A, -D, or -H (see --help "
                 "for details)");
      return EndCondition_t::CliCommandComplete;
    }
  }

  // Configure ground elevation
  if (options.e) {
    char* endPtr;
    double elevation = strtod(options.e, &endPtr);
    if (*endPtr != '\0') {
      cliSendAck(false, "Ground elevation invalid float");
      return EndCondition_t::CliCommandComplete;
    }
    cliGetConfigs()->groundElevationM = elevation;
    // Write new cli configs to flash
    data_log_write_cli_configs();
  }

  // Configure ground temperature
  if (options.t) {
    char* endPtr;
    double temperature = strtod(options.t, &endPtr);
    if (*endPtr != '\0') {
      cliSendAck(false, "Ground temperature invalid float");
      return EndCondition_t::CliCommandComplete;
    }
    cliGetConfigs()->groundTemperatureC = temperature;
    // Write new cli configs to flash
    data_log_write_cli_configs();
  }

  // Configure radio channel
  if (options.c) {
    char* endPtr;
    errno = 0;
    int channel = strtol(options.c, &endPtr, 0);
    if (*endPtr != '\0' || (errno != 0 && channel == 0)) {
      cliSendAck(false, "Invalid channel integer");
      return EndCondition_t::CliCommandComplete;
    }
    cliGetConfigs()->radioChannel = channel;
    // Write new cli configs to flash
    data_log_write_cli_configs();
#ifdef TELEMETRY_RADIO
    HM_RadioSetChannel(TELEMETRY_RADIO, channel);
#endif  // TELEMETRY_RADIO
  }

  // Send positive ACK (all inputs have been appropriately processed)
  cliSendAck(true, nullptr);

  // Send help message to cli
  if (options.h) {
    char name[30];
    char val[40];
    // New line
    cliSend("\r\n");
    // Pyros
    for (int i = 0; i < MAX_PYRO; i++) {
      snprintf(name, sizeof(name), "Pyro %i Configuration:", i + 1);
      PyroConfig_t* pyroConfig = (cliGetConfigs()->pyroConfiguration + i);
      if (pyroConfig->flags == FLAG_APOGEE) {  // Apogee
        snprintf(val, sizeof(val), "Deploy at apogee");
      } else if (pyroConfig->flags ==
                 FLAG_ALT_DURING_DESCENT) {  // Deploy at an altitude
        snprintf(val, sizeof(val), "Deploy at descent altitude of %.2f m",
                 pyroConfig->configValue);
      } else if (pyroConfig->flags == FLAG_APOGEE_DELAY) {
        snprintf(val, sizeof(val), "Deploy %0.2f seconds after apogee",
                 pyroConfig->configValue);
      } else {
        snprintf(val, sizeof(val), "None");
      }
      generateConfigHelp(name, val);
    }
    // Ground elevation
    snprintf(val, sizeof(val), "%.3f", cliGetConfigs()->groundElevationM);
    generateConfigHelp("Ground Elevation (m):", val);
    // Ground temperature
    snprintf(val, sizeof(val), "%.3f", cliGetConfigs()->groundTemperatureC);
    generateConfigHelp("Ground Temperature (C):", val);
    // Radio channel
    snprintf(val, sizeof(val), "%i", cliGetConfigs()->radioChannel);
    generateConfigHelp("Radio Channel:", val);
  }

  // If reached, send complete message to CLI
  cliSendComplete(true, nullptr);
  return EndCondition_t::CliCommandComplete;
}

void CliConfigState::cleanup() {
  // No action
}
