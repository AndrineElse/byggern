#include "../include/gameMenu.h"
#include "../../drivers/include/OLEDDriver.h"
#include "../../drivers/include/ADCDriver.h"



void menuLoop(Node* startNode){
  uint8_t selectedOption = 0;
  Node* currentNode = startNode;
  printNode(currentNode, 1);
}

void printNode(Node* node, uint8_t selectedOption){
  OLED_clear();
  OLED_pos(0,0);
  OLED_print (node.description);
  for (int i = 0; i < node.numOptions; i++){
    OLED_pos(i+1,0);
    if (i == selectedOption){
      OLED_print('*');
    }
    OLED_print(node.options[i]);
  }
}
