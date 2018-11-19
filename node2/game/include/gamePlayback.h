struct game_playback_container {
	//should receive a new reference every 20 ms, this should be ample for 2 minutes of play
	uint8_t controller_reference_timeseries[1200];
	int8_t servo_reference_timeseries[1200];
	uint8_t solenoid_trigger_timeseries[150]; //1/8th here since solenoid is binary
	uint16_t current_filler_index;
	uint16_t current_playback_index;
	uint8_t finished_playing;
};

struct playback_sample_set_container {
	uint8_t controller_reference;
	int8_t servo_reference;
	uint8_t solenoid_trigger;
};

void playback_reset();
void set_next_sample(uint8_t position_reference, int8_t servo_reference, uint8_t solenoid_trigger);
struct playback_sample_set_container playback_get_next_sample();
uint8_t playback_get_finished_playing();
void playback_stop_playing();