
struct Node {
  volatile struct Node* parent;
  char* options[4];
  volatile struct Node* optionNodes[4];
  char* description;
  uint8_t numOptions;
};


void menuLoop();
void menuInit();
void printNodeUsingBuffer(volatile struct Node* node, uint8_t selectedOption);
void game_level_select(uint8_t selected_option);
void game_username_select(uint8_t selectedOption);
void game_send_data_CAN();
void game_highscore_SRAM_update(uint8_t user, uint16_t score, uint8_t place);
uint8_t get_play_game();
void set_play_game(uint8_t value);
void set_username(uint8_t name);
uint8_t get_username();
//void print_highscore_node(uint8_t place, uint8_t username, uint8_t score_H, uint8_t score_L);
uint8_t* game_highscore_update();
