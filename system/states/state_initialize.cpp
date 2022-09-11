
#include "state_initialize.h"

#include "board_config_common.h"
#include "cli.h"
#include "data_log.h"
#include "filters.h"
#include "hardware_manager.h"
#include "radio_manager.h"

#if HAS_DEV(LINE_CUTTER_BLE)
#include "line_cutter_ble.h"
#endif  // HAS_DEV(LINE_CUTTER_BLE)

void InitializeState::init() {
  HM_HardwareInit();

  // This is needed to tell the Kalman filter the nominal dt for prediction
  filterInit(this->period_ms_ / 1000.0);

  // Initiliaze radio circular buffers and things
  RadioManager_init();

  data_log_init();
  data_log_load_cli_configs();

  // Start CLI, which will run in the background in other states
  cliInit();

  // Set up line cutters to forward strings to radio
#if HAS_DEV(LINE_CUTTER_BLE)
  lineCutterBle_registerForwardStringCb(RadioManager_transmitStringDefault);
#endif  // HAS_DEV(LINE_CUTTER_BLE)
}

EndCondition_e InitializeState::run() {
  if (HM_UsbIsConnected(USB_CLI_ID)) {
    return EndCondition_e::UsbConnect;
  }
  return EndCondition_e::UsbDisconnect;
}

void InitializeState::cleanup() {
  // Do nothing
}
