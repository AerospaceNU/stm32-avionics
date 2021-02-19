
#include "buzzer_song.h"

void buzzerSong(BuzzerCtrl_t* buzzer) {

	buzzerSetFrequency(buzzer, 1174.66);
	buzzerStart(buzzer);
	HAL_Delay(250);

	buzzerStop(buzzer);
	HAL_Delay(250);

	buzzerSetFrequency(buzzer, 1396.91);
	buzzerStart(buzzer);
	HAL_Delay(250);

	buzzerStop(buzzer);
	HAL_Delay(125);

	buzzerSetFrequency(buzzer, 1174.66);
	buzzerStart(buzzer);
	HAL_Delay(125);

	buzzerStop(buzzer);
	HAL_Delay(125);

	buzzerSetFrequency(buzzer, 1174.66);
	buzzerStart(buzzer);
	HAL_Delay(125);

	buzzerSetFrequency(buzzer, 1567.98);
	HAL_Delay(125);

	buzzerStop(buzzer);
	HAL_Delay(125);

	buzzerSetFrequency(buzzer, 1174.66);
	buzzerStart(buzzer);
	HAL_Delay(125);

	buzzerStop(buzzer);
	HAL_Delay(125);

	buzzerSetFrequency(buzzer, 1046.5);
	buzzerStart(buzzer);
	HAL_Delay(125);

	buzzerStop(buzzer);
	HAL_Delay(125);

	buzzerSetFrequency(buzzer, 1174.66);
	buzzerStart(buzzer);
	HAL_Delay(250);

	buzzerStop(buzzer);
	HAL_Delay(250);

	buzzerSetFrequency(buzzer, 1760);
	buzzerStart(buzzer);
	HAL_Delay(250);

	buzzerStop(buzzer);
	HAL_Delay(125);

	buzzerSetFrequency(buzzer, 1174.66);
	buzzerStart(buzzer);
	HAL_Delay(125);

	buzzerStop(buzzer);
	HAL_Delay(125);

	buzzerSetFrequency(buzzer, 1174.66);
	buzzerStart(buzzer);
	HAL_Delay(125);

	buzzerSetFrequency(buzzer, 1864.66);
	HAL_Delay(125);

	buzzerStop(buzzer);
	HAL_Delay(125);

	buzzerSetFrequency(buzzer, 1760);
	buzzerStart(buzzer);
	HAL_Delay(125);

	buzzerStop(buzzer);
	HAL_Delay(125);

	buzzerSetFrequency(buzzer, 1396.91);
	buzzerStart(buzzer);
	HAL_Delay(125);

	buzzerStop(buzzer);
	HAL_Delay(125);

	buzzerSetFrequency(buzzer, 1174.66);
	buzzerStart(buzzer);
	HAL_Delay(125);

	buzzerStop(buzzer);
	HAL_Delay(125);

	buzzerSetFrequency(buzzer, 1760);
	buzzerStart(buzzer);
	HAL_Delay(125);

	buzzerStop(buzzer);
	HAL_Delay(125);

	buzzerSetFrequency(buzzer, 2349.32);
	buzzerStart(buzzer);
	HAL_Delay(125);

	buzzerStop(buzzer);
	HAL_Delay(125);

	buzzerSetFrequency(buzzer, 1174.66);
	buzzerStart(buzzer);
	HAL_Delay(125);

	buzzerSetFrequency(buzzer, 1046.5);
	HAL_Delay(125);

	buzzerStop(buzzer);
	HAL_Delay(125);

	buzzerSetFrequency(buzzer, 1046.5);
	buzzerStart(buzzer);
	HAL_Delay(125);

	buzzerSetFrequency(buzzer, 880);
	HAL_Delay(125);

	buzzerStop(buzzer);
	HAL_Delay(125);

	buzzerSetFrequency(buzzer, 1318.51);
	buzzerStart(buzzer);
	HAL_Delay(125);

	buzzerStop(buzzer);
	HAL_Delay(125);

	buzzerSetFrequency(buzzer, 1174.66);
	buzzerStart(buzzer);
	HAL_Delay(1000);

	buzzerStop(buzzer);
	HAL_Delay(1375);
}


