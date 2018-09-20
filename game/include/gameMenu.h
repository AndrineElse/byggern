struct Node typedef Node;
struct Node {
  Node* parent;
  char* options[5];
  Node* optionNodes[5];
  char* description;
  uint8_t numOptions;
} typedef Node;

void menuLoop(Node* startNode);
void printNode(Node* node, uint8_t selectedOption);
