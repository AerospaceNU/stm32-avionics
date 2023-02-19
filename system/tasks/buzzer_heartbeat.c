
#include "buzzer_heartbeat.h"

#include "hardware_manager.h"

static uint32_t last_time = 0;
static bool isOn;
static int buzzerId_;

void buzzerHeartbeat_setBuzzer(int buzzerId) { buzzerId_ = buzzerId; }

// Alternates the buzzer between on and off states if a certain period of time
// has passed (500ms)
void buzzerHeartbeat_tick() {
  if ((hm_millis() - last_time) > 500) {
    last_time = hm_millis();
    if (isOn) {
      hm_buzzerSetFrequency(buzzerId_, 600);
      hm_buzzerStart(buzzerId_);
    } else {
      hm_buzzerStop(buzzerId_);
    }
    isOn = !isOn;
  }
}
