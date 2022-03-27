
#include "buzzer_report_scheme.h"

#include "hardware_manager.h"

static uint32_t last_time = 0;
static uint32_t curState = 0;
extern bool hardwareStatus[NUM_HARDWARE];

/*Plays a different frequency for each init in hardware_manager given that it
 * returns true. If an init returns false, its tone will not play.)*/
void buzzerReport() {
  if (((HM_Millis() - last_time) > 1000)) {
    last_time = HM_Millis();

    if (curState >= NUM_HARDWARE) {
      curState = 0;
    }
    HM_BuzzerSetFrequency(500 + curState * 50);

    if (hardwareStatus[curState] == true) {
      HM_BuzzerStart();
    } else {
      HM_BuzzerStop();
    }
    curState++;
  }
}
