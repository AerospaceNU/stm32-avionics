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

static char msg[270 + 4];

static void generateConfigHelp(const char* name, const char* value) {
  snprintf(msg, sizeof(msg), "%-30s %-240s\r\n", name, value);
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

    if (options.D) {
      triggerConfig->mode = TRIGGER_TYPE_EMPTY;
      triggerManager_removeTrigger(triggerNum);
      // Write new cli configs to flash
      dataLog_writeCliConfigs();
      cli_sendAck(true, nullptr);
      cli_sendComplete(true, nullptr);
      return;
    }

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
      case TRIGGER_TYPE_PWM_PYRO:
        maxPort = NUM_PYRO;
        break;
      case TRIGGER_TYPE_LINE_CUTTER:
        maxPort = NUM_LINE_CUTTER;  // Max number of line cutters, 0 indexed
        break;
      default:
        cli_sendAck(false, "Invalid trigger mode");
        return;
    }

    float duration = 0;

    if (mode == TRIGGER_TYPE_PYRO || mode == TRIGGER_TYPE_PWM_PYRO) {
      duration = smallStrtod(options.d, &endPtr) * 1000;  // Convert to ms
      if (*endPtr != '\0') {
        cli_sendAck(false, "Pyro/PWM must specify a duration");
        return;
      }
    }

    int pulseWidth = 0;

    if (mode == TRIGGER_TYPE_PWM_PYRO) {
      pulseWidth = strtol(options.w, &endPtr, 10);
      if (*endPtr != '\0') {
        cli_sendAck(false, "PWM must specify a pulse width");
        return;
      }
    }

    triggerConfig->duration = duration;
    triggerConfig->pulseWidth = pulseWidth;
    triggerConfig->mode = mode;

    int port = strtol(options.p, &endPtr, 10);
    if (*endPtr != '\0' || port < 0 || port >= maxPort) {
      cli_sendAck(false, "Invalid port number");
      return;
    }

    triggerConfig->port = port;

    if (options.C) {
      const char* configString = options.C;
      if (!triggerManager_setTriggerConfig(triggerNum, &configString)) {
        cli_sendAck(false, "Invalid config string");
        return;
      }
    } else {
      cli_sendAck(false,
                  "Trigger config must specify a condition (see --help "
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
    static char name[30];
    static char val[240];

    // New line
    cli_send("\r\n");
    // Print all triggers
    for (int i = 0; i < MAX_TRIGGER; i++) {
      snprintf(name, sizeof(name), "Trigger %i Configuration:", i);
      TriggerConfig_s* triggerConfig =
          (cli_getConfigs()->triggerConfiguration + i);

      static char deviceText[40];
      bool format = true;
      switch (triggerConfig->mode) {
        case TRIGGER_TYPE_EMPTY:
          generateConfigHelp(name, "none");
          format = false;
          break;
        case TRIGGER_TYPE_PYRO:
          snprintf(
              deviceText, sizeof(deviceText), "Fire pyro %d for %ds on ",
              triggerConfig->port,
              (int)(triggerConfig->duration / 1000.0));  // Convert back to s
          break;
        case TRIGGER_TYPE_LINE_CUTTER:
          snprintf(deviceText, sizeof(deviceText), "Cut line cutter %d on ",
                   triggerConfig->port);
          break;
        case TRIGGER_TYPE_DIGITAL_ON_PYRO:
          snprintf(deviceText, sizeof(deviceText), "Enable digital pin %d on ",
                   triggerConfig->port);
          break;
        case TRIGGER_TYPE_DIGITAL_OFF_PYRO:
          snprintf(deviceText, sizeof(deviceText), "Disable digital pin %d on ",
                   triggerConfig->port);
          break;
        case TRIGGER_TYPE_PWM_PYRO:
          snprintf(
              deviceText, sizeof(deviceText),
              "PWM on pyro %d for %ds with %ld width on", triggerConfig->port,
              (int)(triggerConfig->duration / 1000.0),  // Convert back to ms
              triggerConfig->pulseWidth);
          break;
        default:
          snprintf(deviceText, sizeof(deviceText), "error");
          break;
      }

      static char configText[200];
      if (format) {
        triggerManager_getConfigString(i, configText, 200);
        snprintf(val, sizeof(val), "%s%s", deviceText, configText);
        generateConfigHelp(name, val);
      }
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
