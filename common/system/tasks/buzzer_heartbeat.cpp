#include "buzzer_heartbeat.h"

#include "hardware_manager.h"

static uint32_t last_time = 0;
static int buzzerId_ = 0;
const int goodFreq = 1000;
const int badFreq = 500;

void buzzerHeartbeat_setBuzzer(int buzzerId) { buzzerId_ = buzzerId; }

// Beeps out PYRO Connectivity
void pyroSoundOff(bool pyroContCheck, int& pyroCounter) {
  static bool on = false;
  static int beepCounter = 0;
  if (on) {
    on = !on;
    hm_buzzerStop(buzzerId_);
  } else {
    if (beepCounter <= pyroCounter) {
      hm_buzzerSetFrequency(buzzerId_, pyroContCheck ? goodFreq : badFreq);
      hm_buzzerStart(buzzerId_);
      on = true;
      beepCounter++;
    } else {
      beepCounter = 0;
      pyroCounter++;
    }
  }
}

void buzzerHeartbeat_tick(BuzzerState state) {
	return;
  static int pyroCounter = 0;
  bool pyroContCheck;

  if ((hm_millis() - last_time) > 500) {
    SensorData_s* sensorData = hm_getSensorData();

    switch (state) {
      case BuzzerState::PYRO:
#if HAS_DEV(PYRO_CONT)
        if (pyroCounter >= NUM_PYRO_CONT) {
          pyroCounter = 0;
        }
        pyroContCheck = sensorData->pyroContData[pyroCounter];
        pyroSoundOff(pyroContCheck, pyroCounter);
#endif  // HAS_DEV(PYRO_CONT)
        break;
      default:
        break;
    }
    last_time = hm_millis();
  }
}
