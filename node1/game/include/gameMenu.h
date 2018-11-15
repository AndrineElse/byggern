struct Node {
  struct Node* parent;
  char* options[5];
  struct Node* optionNodes[5];
  char* description;
  uint8_t numOptions;
};

void menuLoop(struct Node* startNode);
void printNode(struct Node* node, uint8_t selectedOption);
void menuInit(struct Node* mainMenuNode);
void printNodeUsingBuffer(struct Node* node, uint8_t selectedOption);
void game_send_update_CAN(struct Game_status* game);
struct Node* getEndGameNode(struct Node* startNode);
struct Node* getMiddleGameNode(struct Node* startNode);
