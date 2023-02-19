#include "state_post_flight.h"

#include "buzzer_heartbeat.h"
#include "cli.h"
#include "cli_tasks.h"
#include "data_log.h"
#include "filters.h"
#include "hardware_manager.h"
#include "state_log.h"
#include "trigger_manager.h"

#define SIM_NO_DATA_TIMEOUT_MS \
  1000  // If no data comes in for this long, stop sim

void PostFlightState::init() {
  // Write that the flight is completed so that we don't jump back to a flight
  // state
  triggerManager_setTouchdownTime(hm_millis());
  stateLog_writeComplete();
  lastSimDataTime = hm_millis();
  CliTasks::configureForGround();
}

EndCondition_e PostFlightState::run() {
  // Run buzzer heartbeat
  buzzerHeartbeat_tick();

  // Check if data is still coming in (sim mode)
  uint32_t curTime = hm_millis();
  if (hm_inSimMode()) {
    if (cb_count(cli_getRxBuffer()) >= sizeof(SensorData_s)) {
      lastSimDataTime = curTime;
    }
    if (curTime - lastSimDataTime > SIM_NO_DATA_TIMEOUT_MS) {
      return EndCondition_e::CliCommandComplete;
    }
  }

  // Collect and transmit data
  SensorData_s* sensorData = hm_getSensorData();
  FilterData_s* filterData = filter_getData();

  // Log data at 1/10th rate for now in case this state is reached before it is
  // supposed to
  if (this->getRunCounter() % 10 == 0)
    dataLog_write(sensorData, filterData, this->getID());

  return EndCondition_e::NoChange;
}

void PostFlightState::cleanup() {
  // If in sim mode, finish it up and release any unread bytes
  if (hm_inSimMode()) {
    cli_sendComplete(true, nullptr);
    hm_disableSimMode();
    cb_flush(cli_getRxBuffer());
  }
}
