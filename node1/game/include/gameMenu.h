struct Node {
  volatile struct Node* parent;
  char* options[5];
  volatile struct Node* optionNodes[5];
  char* description;
  uint8_t numOptions;
};

void menuLoop();
void printNode(struct Node* node, uint8_t selectedOption);
void menuInit();
void printNodeUsingBuffer(volatile struct Node* node, uint8_t selectedOption);


/*
struct Node* getEndGameNode();
struct Node* getMiddleGameNode();
*/
