#include <cerrno>
#include <cinttypes>
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

static char msg[110 + 4];

static void generateConfigHelp(const char* name, const char* value) {
  snprintf(msg, sizeof(msg), "%-30s %-80s\r\n", name, value);
  cli_send(msg);
}

void CliTasks::config() {
  // Get command line options
  CliOptionVals_s options = cli_getOptions();
  // Configure triggers
  char* endPtr;

  if (options.t) {
    // Configuring triggers
    int triggerNum = strtol(options.t, &endPtr, 10);
    if (*endPtr != '\0' || triggerNum < 0 || triggerNum >= MAX_TRIGGER) {
      cli_sendAck(false, "Invalid trigger number");
      return;
    }

    TriggerConfig_s* triggerConfig =
        cli_getConfigs()->triggerConfiguration + triggerNum;
    // Get trigger type
    if (!options.m) {
      cli_sendAck(false, "-m flag must be provided");
      return;
    }
    if (!options.p) {
      cli_sendAck(false, "-p flag must be provided");
      return;
    }
    int mode = strtol(options.m, &endPtr, 10);
    if (*endPtr != '\0') {
      cli_sendAck(false, "Invalid trigger mode");
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
        maxPort = NUM_LINE_CUTTER;  // Max number of line cutters, 0 indexed
        break;
      default:
        cli_sendAck(false, "Invalid trigger mode");
        return;
    }

    triggerConfig->mode = mode;

    int port = strtol(options.p, &endPtr, 10);
    if (*endPtr != '\0' || port < 0 || port >= maxPort) {
      cli_sendAck(false, "Invalid port number");
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
    } else if (options.C) {
      triggerConfig->flags = FLAG_CUSTOM_MARMAN_CLAMP;
    } else if (options.U) {
      triggerConfig->flags = FLAG_CUSTOM_MARMAN_DELAY;
    } else if (options.H) {
      double pyroAlt = smallStrtod(options.H, &endPtr);
      if (*endPtr != '\0') {
        cli_sendAck(false, "Trigger deploy altitude invalid float");
        return;
      }
      triggerConfig->flags = FLAG_ALT_DURING_DESCENT;
      triggerConfig->configValue = pyroAlt;
    } else if (options.D) {
      double apogeeDelay = smallStrtod(options.D, &endPtr);
      if (*endPtr != '\0') {
        cli_sendAck(false, "Apogee delay invalid float");
        return;
      }
      triggerConfig->flags = FLAG_APOGEE_DELAY;
      triggerConfig->configValue = apogeeDelay;
    } else {
      cli_sendAck(false,
                  "Trigger config must either specify a condition (see --help "
                  "for details)");
      return;
    }
    // Write new cli configs to flash
    dataLog_writeCliConfigs();
  }

  // Configure ground elevation
  if (options.e) {
    double elevation = smallStrtod(options.e, &endPtr);
    if (*endPtr != '\0') {
      cli_sendAck(false, "Ground elevation invalid float");
      return;
    }
    cli_getConfigs()->groundElevationM = elevation;
    // Write new cli configs to flash
    dataLog_writeCliConfigs();
  }

  // Configure ground temperature
  if (options.r) {
    double temperature = smallStrtod(options.r, &endPtr);
    if (*endPtr != '\0') {
      cli_sendAck(false, "Ground temperature invalid float");
      return;
    }
    cli_getConfigs()->groundTemperatureC = temperature;
    // Write new cli configs to flash
    dataLog_writeCliConfigs();
  }

  // Configure radio channel
  if (options.c) {
    errno = 0;
    int channel = strtol(options.c, &endPtr, 0);
    if (*endPtr != '\0' || (errno != 0 && channel == 0)) {
      cli_sendAck(false, "Invalid channel integer");
      return;
    }
    cli_getConfigs()->radioChannel = channel;
    // Write new cli configs to flash
    dataLog_writeCliConfigs();
    // For now, set all channels since there's no support for multiple radios
    for (int i = 0; i < NUM_RADIO; i++) {
      hm_radioSetChannel(i, channel);
    }
  }

  // Send positive ACK (all inputs have been appropriately processed)
  cli_sendAck(true, nullptr);

  // Send help message to cli
  if (options.h) {
    char name[30];
    char val[80];
    char float_buff[10];
    // New line
    cli_send("\r\n");
    // Print all triggers
    for (int i = 0; i < MAX_TRIGGER; i++) {
      snprintf(name, sizeof(name), "Trigger %i Configuration:", i);
      TriggerConfig_s* triggerConfig =
          (cli_getConfigs()->triggerConfiguration + i);
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
        case FLAG_CUSTOM_MARMAN_CLAMP:
          snprintf(val, sizeof(val), "%s %i under 20 m/s before apogee",
                   deviceText, triggerConfig->port);
          break;
        case FLAG_CUSTOM_MARMAN_DELAY:
          snprintf(val, sizeof(val), "%s %i 1s after first marman cut",
                   deviceText, triggerConfig->port);
          break;
        default:
          snprintf(val, sizeof(val), "None");
      }

      generateConfigHelp(name, val);
    }
    // Ground elevation
    dtoa(val, sizeof(val), cli_getConfigs()->groundElevationM, 3);
    generateConfigHelp("Ground Elevation (m):", val);
    // Ground temperature

    dtoa(val, sizeof(val), cli_getConfigs()->groundTemperatureC, 3);
    generateConfigHelp("Ground Temperature (C):", val);
    // Radio channel
    snprintf(val, sizeof(val), "%" PRIi32, cli_getConfigs()->radioChannel);
    generateConfigHelp("Radio Channel:", val);
  }

  // If reached, send complete message to CLI
  cli_sendComplete(true, nullptr);
  return;
}
