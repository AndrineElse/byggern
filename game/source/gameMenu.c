#include <stdint.h>
#include "../include/gameMenu.h"
#include "../../drivers/include/OLEDDriver.h"
#include "../../drivers/include/userInputDriver.h"
#include <util/delay.h>

void menuInit(struct Node* mainMenuNode){

  //struct Node mainMenuNode;
  struct Node playGameNode;
  playGameNode.parent = mainMenuNode;
  playGameNode.options[0] = "Go back";
  playGameNode.description = "Game";
  playGameNode.numOptions = 1;

  struct Node highScoresNode;
  highScoresNode.parent = mainMenuNode;
  highScoresNode.options[0] = "Go back";
  highScoresNode.description = "highscore";
  highScoresNode.numOptions = 1;

  struct Node optionsNode;
  optionsNode.parent = mainMenuNode;
  optionsNode.options[0] = "Go back";
  optionsNode.description = "options";
  optionsNode.numOptions = 1;


  mainMenuNode->parent = (struct Node*)0;
  mainMenuNode->options[0] = "Play game";
  mainMenuNode->options[1] = "Highscores";
  mainMenuNode->options[2] = "Options";

  mainMenuNode->description = "This is the main menu :)";
  mainMenuNode->numOptions = 3;

  mainMenuNode->optionNodes[0] = &playGameNode;
  mainMenuNode->optionNodes[1] = &highScoresNode;
  mainMenuNode->optionNodes[2] = &optionsNode;

  //mainMenuNode = &mainMenuNode;
}

void menuLoop(struct Node* startNode){

  uint8_t selectedOption = 0;
  JoystickDir currentDir;
  struct Node* currentNode = startNode;
  JoystickDir lastDir;
  lastDir = calculateJoystickDirection();
  uint8_t lastButtonValue = 0;
  while(1){
    //get joystick input
    JoystickOffset joystickOffset;
    joystickOffset = calculateOffsetJoystick();
    JoystickCoords joystickCoords;
    joystickCoords = calculateCalibratedJoystickCoords(joystickOffset);
    JoystickDir currentDir;
    currentDir = calculateJoystickDirection(joystickCoords);

    //find selected option

    if (currentDir != lastDir){
      if (currentDir == UP){
        if (selectedOption > 0){
          selectedOption = selectedOption -1;
        }
      }
      else if (currentDir == DOWN){
        if (selectedOption < (currentNode->numOptions-1)){
          selectedOption = selectedOption +1;
        }
      }
      //evt modulo numOptions her istedet
      lastDir = currentDir;
    }

    //NB, this requires selectedoptions to be < length(options)
    if (!lastButtonValue && joystickButton()) {
      currentNode = currentNode->optionNodes[selectedOption];
      selectedOption = 0;
      OLED_buffer_clear();
    }

    lastButtonValue = joystickButton();
    _delay_ms(50);
    printNodeUsingBuffer(currentNode, selectedOption);
    OLED_update_screen_from_buffer();
  }
}

void printNode(struct Node* node, uint8_t selectedOption){
  OLED_clear();
  OLED_pos(0,0);
  OLED_print (node->description);

  for (int i = 0; i < node->numOptions; i++){
    OLED_pos(i+1,0);
    if (i == selectedOption){
      OLED_print("* ");
    }
    OLED_print(node->options[i]);

  }
  _delay_ms(500);
}

void printNodeUsingBuffer(struct Node* node, uint8_t selectedOption){

  OLED_buffer_print_line (node->description,0,0);

  for (int i = 0; i < node->numOptions; i++){
    if (i == selectedOption){
      OLED_buffer_print_line(node->options[i], i+1, 1);
    }
    else {
      OLED_buffer_print_line(node->options[i],i+1,0);
    }
  }
}
