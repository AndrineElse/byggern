struct Game_status {
  uint8_t fails;
  uint16_t timer;
  uint8_t lives;
};

void game_loop(struct IR_status* IR_sample_container);
void count_game_score(struct Game_status* game,struct IR_status* IR_sample_container);
