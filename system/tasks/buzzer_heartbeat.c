
#include "buzzer_heartbeat.h"

#include "hardware_manager.h"

static uint32_t last_time = 0;
static bool isOn;
static int buzzerId_;

void buzzerHeartbeatSetBuzzer(int buzzerId) { buzzerId_ = buzzerId; }

// Alternates the buzzer between on and off states if a certain period of time
// has passed (500ms)
void buzzerHeartbeat() {
  if ((HM_Millis() - last_time) > 500) {
    last_time = HM_Millis();
    if (isOn) {
      HM_BuzzerSetFrequency(buzzerId_, 600);
      HM_BuzzerStart(buzzerId_);
    } else {
      HM_BuzzerStop(buzzerId_);
    }
    isOn = !isOn;
  }
}
