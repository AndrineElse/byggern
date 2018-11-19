struct Game_status {
  uint8_t fails;
  uint16_t timer;
  uint8_t lives;
  uint16_t score;
  uint8_t playing;
  uint8_t fail_detected;
};


void game_status_container_init();
volatile struct Game_status* game_status_container_get_ptr();
void game_status_container_update(struct CAN_msg new_game_message);
