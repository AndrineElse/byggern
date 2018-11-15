struct Game_status {
  uint8_t fails;
  uint16_t timer;
  uint8_t lives;
  uint32_t score;
};

void game_loop(struct IR_status* IR_sample_container, struct PID_data* pid);
void count_game_score(struct Game_status* game,struct IR_status* IR_sample_container);
