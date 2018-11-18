
struct Node {
  volatile struct Node* parent;
  char* options[4];
  volatile struct Node* optionNodes[4];
  char* description;
  uint8_t numOptions;
};

struct GameData {
  uint8_t gameStart;
  uint8_t pause;
  uint8_t calibrateEncoder;
};

void menuLoop();
void printNode(volatile struct Node* node, uint8_t selectedOption);
void menuInit();
void printNodeUsingBuffer(volatile struct Node* node, uint8_t selectedOption);
void game_level_select(uint8_t selected_option);
void game_username_select(uint8_t selectedOption);
void game_send_data_CAN();
void game_highscore_SRAM_update(uint8_t user, uint16_t score, uint8_t place);
