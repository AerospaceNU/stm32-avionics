
#include "state_initialize.h"

#include "board_config_common.h"
#include "cli.h"
#include "data_log.h"
#include "event_manager.h"
#include "filters.h"
#include "hardware_manager.h"
#include "radio_manager.h"

#if HAS_DEV(LINE_CUTTER_BLE)
#include "line_cutter_ble.h"
#endif  // HAS_DEV(LINE_CUTTER_BLE)

void InitializeState::init() {
  hm_hardwareInit();

  // This is needed to tell the Kalman filter the nominal dt for prediction
  filter_init(this->period_ms_ / 1000.0);

  // Initiliaze radio circular buffers and things
  radioManager_init();

  dataLog_init();
  dataLog_loadCliConfigs();

  // Start CLI, which will run in the background in other states
  cli_init();

  eventManager_init();

  // Set up line cutters to forward strings to radio
#if HAS_DEV(LINE_CUTTER_BLE)
  lineCutterBle_registerForwardStringCb(radioManager_transmitStringDefault);
#endif  // HAS_DEV(LINE_CUTTER_BLE)
}

EndCondition_e InitializeState::run() { return EndCondition_e::Initialized; }

void InitializeState::cleanup() {
  // Do nothing
}
