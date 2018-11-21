struct Node {
  volatile struct Node* parent;
  char* options[4];
  volatile struct Node* optionNodes[4];
  char* description;
  uint8_t numOptions;
};

struct Highscore {
  uint8_t users[3] = {3,3,3};
  uint16_t scores[3] = {0,0,0};
};

void menuLoop();
void menuInit();
void printNodeUsingBuffer(volatile struct Node* node, uint8_t selectedOption);
void game_level_select(uint8_t selected_option);
void game_send_data_CAN();
uint8_t get_play_game();
void set_play_game(uint8_t value);
void set_username(uint8_t name);
void print_highscore_node(uint8_t place, uint8_t username, uint16_t score);
void game_highscore_update();
uint8_t get_restart_game();
