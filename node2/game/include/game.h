struct Game_status {
  uint8_t fails;
  uint16_t timer;
  uint8_t lives;
  uint16_t score;
  uint8_t playing;
  uint8_t fail_detected;
  uint8_t running_playback;
};

void game_init();
void game_loop();
void count_game_score();
void game_send_update_CAN();
uint8_t  game_get_playing_status();
uint8_t game_get_playback_status();
void game_select_controller(struct CAN_msg new_input_message);
