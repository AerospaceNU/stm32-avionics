
#include "buzzer_heartbeat.h"
#include "hardware_manager.h"

static uint32_t last_time = 0;
static bool isOn;

//Alternates the buzzer between on and off states if a certain period of time has passed (500ms)
void buzzerHeartbeat(){
        if ((HM_Millis() - last_time) > 500) {
            last_time = HM_Millis();
            if (isOn) {
            	HM_BuzzerSetFrequency(600);
            	HM_BuzzerStart();
            } else {
            	HM_BuzzerStop();
            }
            isOn = !isOn;
        }
}
