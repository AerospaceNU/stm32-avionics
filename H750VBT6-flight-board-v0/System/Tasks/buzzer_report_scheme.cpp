
#include "buzzer_report_scheme.h"
#include "hardware_manager.h"

static uint32_t last_time = HM_Millis();
static uint32_t curState = 0;
static uint32_t n = 0;


//Plays a different frequency for each init in hardware_manager given that it returns true. If an init returns false, its tone will not play.)
void buzzerReport(){
        if ((HM_Millis() - last_time) > 500) {
            last_time = HM_Millis();

        	switch(curState){
    			case 0:
    				HM_BuzzerStop();
    				curState = n++;
    			break;
        		case 1:
        			if(IMU1Status){
        			HM_BuzzerSetFrequency(550);
        			HM_BuzzerStart();}
        			curState = 0;
        			break;
        		case 2:
        			if(IMU2Status){
        			HM_BuzzerSetFrequency(600);
        			HM_BuzzerStart();}
        			curState = 0;
        			break;
        		case 3:
        			if(barometer1Status){
        		    HM_BuzzerSetFrequency(650);
        		    HM_BuzzerStart();}
        		    curState = 0;
        		    break;
        		case 4:
        			if(barometer2Status){
        			HM_BuzzerSetFrequency(700);
        			HM_BuzzerStart();}
        			curState = 0;
        			break;
        		case 5:
        			if(high_G_AccelerometerStatus){
        			HM_BuzzerSetFrequency(750);
        			HM_BuzzerStart();}
        			curState = 0;
        			break;
        		case 6:
        			if(cc1120Status){
        			HM_BuzzerSetFrequency(800);
        			HM_BuzzerStart();}
        			curState = 0;
        			break;
        		default:
        			n = 0;
        			curState = 0;
        			break;
        	}
        }
}
