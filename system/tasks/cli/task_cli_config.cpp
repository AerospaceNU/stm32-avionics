#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "board_config_common.h"
#include "cli.h"
#include "cli_tasks.h"
#include "data_log.h"
#include "dtoa.h"
#include "hardware_manager.h"
#include "small_strtod.h"

static void generateConfigHelp(const char* name, const char* value) {
  char msg[70 + 4];
  snprintf(msg, sizeof(msg), "%-30s %-40s\r\n", name, value);
  cliSend(msg);
}

void cli_tasks::cliConfig() {
  // Get command line options
  CliOptionVals_s options = cliGetOptions();
  // Configure triggers
  char* endPtr;

  if (options.t) {
    // Configuring triggers
    int triggerNum = strtol(options.t, &endPtr, 10);
    if (*endPtr != '\0' || triggerNum < 0 || triggerNum >= MAX_TRIGGER) {
      cliSendAck(false, "Invalid trigger number");
      return;
    }

    TriggerConfig_s* triggerConfig =
        cliGetConfigs()->triggerConfiguration + triggerNum;
    // Get trigger type
    if (!options.m) {
      cliSendAck(false, "-m flag must be provided");
      return;
    }
    if (!options.p) {
      cliSendAck(false, "-p flag must be provided");
      return;
    }
    int mode = strtol(options.m, &endPtr, 10);
    if (*endPtr != '\0') {
      cliSendAck(false, "Invalid trigger mode");
      return;
    }

    int maxPort;
    switch (mode) {
      case TRIGGER_TYPE_PYRO:
      case TRIGGER_TYPE_DIGITAL_ON_PYRO:
      case TRIGGER_TYPE_DIGITAL_OFF_PYRO:
        maxPort = NUM_PYRO;
        break;
      case TRIGGER_TYPE_LINE_CUTTER:
        maxPort = 255;  // Max uint8 value
        break;
      default:
        cliSendAck(false, "Invalid trigger mode");
        return;
    }

    triggerConfig->mode = mode;

    int port = strtol(options.p, &endPtr, 10);
    if (*endPtr != '\0' || port < 0 || port >= maxPort) {
      cliSendAck(false, "Invalid port number");
      return;
    }

    triggerConfig->port = port;

    if (options.A) {
      triggerConfig->flags = FLAG_APOGEE;
    } else if (options.L) {
      triggerConfig->flags = FLAG_LAUNCH;
    } else if (options.T) {
      triggerConfig->flags = FLAG_TOUCHDOWN;
    } else if (options.M) {
      triggerConfig->flags = FLAG_MANUAL;
    } else if (options.H) {
      double pyroAlt = small_strtod(options.H, &endPtr);
      if (*endPtr != '\0') {
        cliSendAck(false, "Trigger deploy altitude invalid float");
        return;
      }
      triggerConfig->flags = FLAG_ALT_DURING_DESCENT;
      triggerConfig->configValue = pyroAlt;
    } else if (options.D) {
      double apogeeDelay = small_strtod(options.D, &endPtr);
      if (*endPtr != '\0') {
        cliSendAck(false, "Apogee delay invalid float");
        return;
      }
      triggerConfig->flags = FLAG_APOGEE_DELAY;
      triggerConfig->configValue = apogeeDelay;
    } else {
      cliSendAck(false,
                 "Trigger config must either specify a condition (see --help "
                 "for details)");
      return;
    }
    // Write new cli configs to flash
    data_log_write_cli_configs();
  }

  // Configure ground elevation
  if (options.e) {
    double elevation = small_strtod(options.e, &endPtr);
    if (*endPtr != '\0') {
      cliSendAck(false, "Ground elevation invalid float");
      return;
    }
    cliGetConfigs()->groundElevationM = elevation;
    // Write new cli configs to flash
    data_log_write_cli_configs();
  }

  // Configure ground temperature
  if (options.r) {
    double temperature = small_strtod(options.r, &endPtr);
    if (*endPtr != '\0') {
      cliSendAck(false, "Ground temperature invalid float");
      return;
    }
    cliGetConfigs()->groundTemperatureC = temperature;
    // Write new cli configs to flash
    data_log_write_cli_configs();
  }

  // Configure radio channel
  if (options.c) {
    errno = 0;
    int channel = strtol(options.c, &endPtr, 0);
    if (*endPtr != '\0' || (errno != 0 && channel == 0)) {
      cliSendAck(false, "Invalid channel integer");
      return;
    }
    cliGetConfigs()->radioChannel = channel;
    // Write new cli configs to flash
    data_log_write_cli_configs();
    // For now, set all channels since there's no support for multiple radios
    for (int i = 0; i < NUM_RADIO; i++) {
      HM_RadioSetChannel(i, channel);
    }
  }

  // Send positive ACK (all inputs have been appropriately processed)
  cliSendAck(true, nullptr);

  // Send help message to cli
  if (options.h) {
    char name[30];
    char val[80];
    char float_buff[10];
    // New line
    cliSend("\r\n");
    // Print all triggers
    for (int i = 0; i < MAX_TRIGGER; i++) {
      snprintf(name, sizeof(name), "Trigger %i Configuration:", i);
      TriggerConfig_s* triggerConfig =
          (cliGetConfigs()->triggerConfiguration + i);
      dtoa(float_buff, sizeof(float_buff), triggerConfig->configValue, 2);
      const char* deviceText;

      switch (triggerConfig->mode) {
        case TRIGGER_TYPE_PYRO:
          deviceText = "Fire pyro";
          break;
        case TRIGGER_TYPE_LINE_CUTTER:
          deviceText = "Cut line cutter";
          break;
        case TRIGGER_TYPE_DIGITAL_ON_PYRO:
          deviceText = "Enable digital pin";
          break;
        case TRIGGER_TYPE_DIGITAL_OFF_PYRO:
          deviceText = "Disable digital pin";
          break;
        default:
          deviceText = "";
          break;
      }

      switch (triggerConfig->flags) {
        case FLAG_APOGEE:
          snprintf(val, sizeof(val), "%s %i at apogee", deviceText,
                   triggerConfig->port);
          break;
        case FLAG_ALT_DURING_DESCENT:
          snprintf(val, sizeof(val), "%s %i at descent altitude of %s m",
                   deviceText, triggerConfig->port, float_buff);
          break;
        case FLAG_APOGEE_DELAY:
          snprintf(val, sizeof(val), "%s %i %s seconds after apogee",
                   deviceText, triggerConfig->port, float_buff);
          break;
        case FLAG_LAUNCH:
          snprintf(val, sizeof(val), "%s %i on launch", deviceText,
                   triggerConfig->port);
          break;
        case FLAG_TOUCHDOWN:
          snprintf(val, sizeof(val), "%s %i on touchdown", deviceText,
                   triggerConfig->port);
          break;
        case FLAG_MANUAL:
          snprintf(val, sizeof(val), "%s %i manually", deviceText,
                   triggerConfig->port);
          break;
        default:
          snprintf(val, sizeof(val), "None");
      }

      generateConfigHelp(name, val);
    }
    // Ground elevation
    dtoa(val, sizeof(val), cliGetConfigs()->groundElevationM, 3);
    generateConfigHelp("Ground Elevation (m):", val);
    // Ground temperature

    dtoa(val, sizeof(val), cliGetConfigs()->groundTemperatureC, 3);
    generateConfigHelp("Ground Temperature (C):", val);
    // Radio channel
    snprintf(val, sizeof(val), "%i", cliGetConfigs()->radioChannel);
    generateConfigHelp("Radio Channel:", val);
  }

  // If reached, send complete message to CLI
  cliSendComplete(true, nullptr);
  return;
}
