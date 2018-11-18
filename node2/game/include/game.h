struct Game_status {
  uint8_t fails;
  uint16_t timer;
  uint8_t lives;
  uint16_t score;
};

void game_loop();
void count_game_score(struct Game_status* game);
void game_select_controller(struct CAN_msg new_input_message);
void game_send_update_CAN(struct Game_status* game, uint16_t* timer, uint8_t* flag);
