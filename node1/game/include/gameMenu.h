
struct Node {
  volatile struct Node* parent;
  char* options[4];
  volatile struct Node* optionNodes[4];
  char* description;
  uint8_t numOptions;
};


void menuLoop();
void printNode(volatile struct Node* node, uint8_t selectedOption);
void menuInit();
void printNodeUsingBuffer(volatile struct Node* node, uint8_t selectedOption);
void game_send_data_CAN();
uint8_t get_play_game();
void set_play_game(uint8_t value);
