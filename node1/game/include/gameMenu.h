
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
void game_send_data_CAN();
