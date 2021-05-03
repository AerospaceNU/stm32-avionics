
#include "buzzer_report_scheme.h"
#include "hardware_manager.h"

static uint32_t hardwareStatusSize = sizeof(hardwareStatus) / sizeof(hardwareStatus[0]);
static uint32_t last_time = HM_Millis();
static uint32_t curState = 0;
static bool isOn = false;

/*Plays a different frequency for each init in hardware_manager given that it returns true. If an init returns false, its tone will not play.)*/
void buzzerReport(){
        if (((HM_Millis() - last_time) > 500)) {
            last_time = HM_Millis();

            if ((curState < hardwareStatusSize)&&(isOn)){
            	HM_BuzzerSetFrequency(500 + curState*50);

            	if (*(hardwareStatus[curState]) == true){
            	HM_BuzzerStart();
            	} else {
            		HM_BuzzerStop();
            	}
            	isOn = !isOn;
            	curState++;
            } else {
            	HM_BuzzerStop();
            	isOn = !isOn;
            }
        }
}
