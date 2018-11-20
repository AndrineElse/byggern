#include "stdint.h"
#include "../include/gamePlayback.h"

volatile struct game_playback_container playback_container;
struct playback_sample_set_container current_sample_container;

void playback_reset() {
	playback_container.current_filler_index = 0;
	playback_container.current_playback_index = 0;
	playback_container.finished_playing = 0;
	for(uint8_t i = 0; i < 150; i++){
		playback_container.solenoid_trigger_timeseries[i] = 0;
	}
	current_sample_container.controller_reference = 0;
	current_sample_container.servo_reference = 0;
	current_sample_container.solenoid_trigger = 0;
}

void playback_set_next_sample(uint8_t position_reference, int8_t servo_reference, uint8_t solenoid_trigger) {

	if(playback_container.current_filler_index >= 1200){
		return;
	}

	uint16_t index = playback_container.current_filler_index;
	playback_container.controller_reference_timeseries[index] = position_reference;
	playback_container.servo_reference_timeseries[index] = servo_reference;
	playback_container.solenoid_trigger_timeseries[index/8] |= (solenoid_trigger << ( index % 8 ));
	playback_container.current_filler_index++;
}

void playback_load_next_sample() {


	uint16_t index = playback_container.current_playback_index;

	if(index >= playback_container.current_filler_index){
		playback_container.finished_playing = 1;
		return;
	}

	struct playback_sample_set_container returnValue;
	returnValue.controller_reference = playback_container.controller_reference_timeseries[index];
	returnValue.servo_reference = playback_container.servo_reference_timeseries[index];
	returnValue.solenoid_trigger = playback_container.servo_reference_timeseries[index/8] & (1 << ( index % 8 ));

	playback_container.current_playback_index++;

	current_sample_container = returnValue;
}

struct playback_sample_set_container playback_get_current_sample() {
	return current_sample_container;
}


uint8_t playback_get_finished_playing() {
	printf("%d\n\r",playback_container.current_filler_index);
	printf("%d\n\r",playback_container.current_playback_index);
	return playback_container.finished_playing;
}

void playback_stop_playing() {
	playback_container.finished_playing = 1;
	playback_container.current_playback_index = 0;
}
