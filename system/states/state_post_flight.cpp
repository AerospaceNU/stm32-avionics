#include "state_post_flight.h"

#include "buzzer_heartbeat.h"
#include "cli.h"
#include "data_log.h"
#include "filters.h"
#include "hardware_manager.h"
#include "state_log.h"

#define SIM_NO_DATA_TIMEOUT_MS \
  1000  // If no data comes in for this long, stop sim

void PostFlightState::init() {
  // Write that the flight is completed so that we don't jump back to a flight
  // state
  state_log_write_complete();
  lastSimDataTime = HM_Millis();
}

EndCondition_e PostFlightState::run() {
  // Run buzzer heartbeat
  buzzerHeartbeat();

  // Check if data is still coming in (sim mode)
  uint32_t curTime = HM_Millis();
  if (HM_InSimMode()) {
    if (cbCount(cliGetRxBuffer()) >= sizeof(SensorData_s)) {
      lastSimDataTime = curTime;
    }
    if (curTime - lastSimDataTime > SIM_NO_DATA_TIMEOUT_MS) {
      return EndCondition_e::CliCommandComplete;
    }
  }

  // Collect and transmit data
  SensorData_s* sensorData = HM_GetSensorData();
  FilterData_s* filterData = filterGetData();

  // Log data at 1/10th rate for now in case this state is reached before it is
  // supposed to
  if (this->getRunCounter() % 10 == 0)
    data_log_write(sensorData, filterData, this->getID());

  // Detect if USB has been plugged in (non-sim mode)
  if (!HM_InSimMode() && HM_UsbIsConnected(USB_CLI_ID)) {
    return EndCondition_e::UsbConnect;
  }

  return EndCondition_e::NoChange;
}

void PostFlightState::cleanup() {
  // If in sim mode, finish it up and release any unread bytes
  if (HM_InSimMode()) {
    cliSendComplete(true, nullptr);
    HM_DisableSimMode();
    cbFlush(cliGetRxBuffer());
  }
}
