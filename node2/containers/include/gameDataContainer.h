struct GameData {
  uint8_t gameStart;
  uint8_t pause;
  uint8_t calibrateEncoder;
};


void game_data_container_init();
volatile struct GameData* game_data_container_get_ptr();
void game_data_container_update(struct CAN_msg new_game_message);
void print_game_data_container();
