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
#if HAS_DEV(PYRO)
  // Configure pyro
  if (options.p) {
    char* endPtr;
    int pyroNum = strtol(options.p, &endPtr, 10) - 1;
    if (*endPtr != '\0' || pyroNum < 0 || pyroNum >= NUM_PYRO) {
      cliSendAck(false, "Invalid pyro number");
      return;
    }
    if (options.A) {
      (cliGetConfigs()->pyroConfiguration + pyroNum)->flags = FLAG_APOGEE;
      // Write new cli configs to flash
      data_log_write_cli_configs();
    } else if (options.H) {
      double pyroAlt = small_strtod(options.H, &endPtr);
      if (*endPtr != '\0') {
        cliSendAck(false, "Pyro deploy altitude invalid float");
        return;
      }
      (cliGetConfigs()->pyroConfiguration + pyroNum)->flags =
          FLAG_ALT_DURING_DESCENT;
      (cliGetConfigs()->pyroConfiguration + pyroNum)->configValue = pyroAlt;
      // Write new cli configs to flash
      data_log_write_cli_configs();
    } else if (options.D) {
      double apogeeDelay = small_strtod(options.D, &endPtr);
      if (*endPtr != '\0') {
        cliSendAck(false, "Apogee delay invalid float");
        return;
      }
      (cliGetConfigs()->pyroConfiguration + pyroNum)->flags = FLAG_APOGEE_DELAY;
      (cliGetConfigs()->pyroConfiguration + pyroNum)->configValue = apogeeDelay;
      // Write new cli configs to flash
      data_log_write_cli_configs();
    } else {
      cliSendAck(false,
                 "Pyro config must either specify -A, -D, or -H (see --help "
                 "for details)");
      return;
    }
  }
#endif  // HAS_DEV(PYRO)

  // Configure ground elevation
  if (options.e) {
    char* endPtr;
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
  if (options.t) {
    char* endPtr;
    double temperature = small_strtod(options.t, &endPtr);
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
    char* endPtr;
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
    char val[45];
    char float_buff[10];
    // New line
    cliSend("\r\n");
#if HAS_DEV(PYRO)
    // Pyros
    for (int i = 0; i < NUM_PYRO; i++) {
      snprintf(name, sizeof(name), "Pyro %i Configuration:", i + 1);
      PyroConfig_s* pyroConfig = (cliGetConfigs()->pyroConfiguration + i);
      dtoa(float_buff, sizeof(float_buff), pyroConfig->configValue, 2);
      if (pyroConfig->flags == FLAG_APOGEE) {  // Apogee
        snprintf(val, sizeof(val), "Deploy at apogee");
      } else if (pyroConfig->flags ==
                 FLAG_ALT_DURING_DESCENT) {  // Deploy at an altitude
        snprintf(val, sizeof(val), "Deploy at descent altitude of %s m",
                 float_buff);
      } else if (pyroConfig->flags == FLAG_APOGEE_DELAY) {
        snprintf(val, sizeof(val), "Deploy %s seconds after apogee",
                 float_buff);
      } else {
        snprintf(val, sizeof(val), "None");
      }
      generateConfigHelp(name, val);
    }
#endif  // HAS_DEV(PYRO)
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
