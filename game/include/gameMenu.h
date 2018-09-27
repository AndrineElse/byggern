struct Node {
  struct Node* parent;
  char* options[5];
  struct Node* optionNodes[5];
  char* description;
  uint8_t numOptions;
};

void menuLoop(struct Node* startNode);
void printNode(struct Node* node, uint8_t selectedOption);
