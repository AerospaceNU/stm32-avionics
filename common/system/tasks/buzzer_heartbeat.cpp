#include "buzzer_heartbeat.h"

static uint32_t last_time = 0;
// static bool isOn;
static int buzzerId_ = 0;
BuzzerState state = BuzzerState::PYRO;

void buzzerHeartbeat_setBuzzer(int buzzerId) { buzzerId_ = buzzerId; }

void pyroSoundOff(bool pyroContCheck, int& pyroCounter);

// Alternates the buzzer between on and off states if a certain period of time
// has passed (500ms)
void buzzerHeartbeat_tick(BuzzerState state) {
  static int pyroCounter = 0;
  bool pyroContCheck;

  if ((hm_millis() - last_time) > 500) {
    SensorData_s* sensorData = hm_getSensorData();

    switch (state) {
      case BuzzerState::PYRO:
        if (pyroCounter >= NUM_PYRO_CONT) {
          pyroCounter = 0;
        }
        pyroContCheck = sensorData->pyroContData[pyroCounter];
        pyroSoundOff(pyroContCheck, pyroCounter);
        break;
      default:
        break;
    }
    last_time = hm_millis();
  }
}

void pyroSoundOff(bool pyroContCheck, int& pyroCounter) {
  static bool on = false;
  static int beepCounter = 0;
  if (on) {
    on = !on;
    hm_buzzerStop(buzzerId_);
  } else {
    if (beepCounter <= pyroCounter) {
      if (pyroContCheck) {
        hm_buzzerSetFrequency(buzzerId_, 1000);
        hm_buzzerStart(buzzerId_);
        on = true;
      } else {
        hm_buzzerSetFrequency(buzzerId_, 300);
        hm_buzzerStart(buzzerId_);
        on = true;
      }
      beepCounter++;
    } else {
      beepCounter = 0;
      pyroCounter++;
    }
  }
}
