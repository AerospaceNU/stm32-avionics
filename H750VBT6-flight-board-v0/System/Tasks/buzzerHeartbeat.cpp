
#include "hardware_manager.h"
#include "buzzerHeartbeat.h"

//Alternates the buzzer between on and off states if a certain period of time has passed (500ms)
void buzzerHeartbeat(){
        if ((HM_Millis() - last_time) > 500) {
            last_time = HM_Millis();
            if (count % 2 == 0) {
            	HM_BuzzerStart();
            } else {
            	HM_BuzzerStop();
            }
            count++;
        }
}
