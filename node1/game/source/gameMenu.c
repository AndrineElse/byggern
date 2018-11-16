#include <stdint.h>
#include "../include/gameMenu.h"
#include "../../drivers/include/OLEDDriver.h"
#include "../../drivers/include/userInputDriver.h"
#include "../../drivers/include/CANDriver.h"
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
  highScoresNode.description = "Highscore";
  highScoresNode.numOptions = 1;

  struct Node optionsNode;
  optionsNode.parent = mainMenuNode;
  optionsNode.options[0] = "Select level";
  optionsNode.options[1] = "Go back";
  optionsNode.description = "Options";
  optionsNode.numOptions = 2;

  struct Node levelsNode;
  optionsNode.parent = optionsNode;
  optionsNode.options[0] = "Easy";
  optionsNode.options[1] = "Medium";
  optionsNode.options[2] = "Hard";
  optionsNode.options[3] = "Go back";
  optionsNode.description = "Level";
  optionsNode.numOptions = 4;


  mainMenuNode->parent = (struct Node*)0;
  mainMenuNode->options[0] = "Play game";
  mainMenuNode->options[1] = "Highscores";
  mainMenuNode->options[2] = "Options";

  mainMenuNode->description = "This is the main menu :)";
  mainMenuNode->numOptions = 3;

  mainMenuNode->optionNodes[0] = &playGameNode;
  mainMenuNode->optionNodes[1] = &highScoresNode;
  mainMenuNode->optionNodes[2] = &optionsNode;

  optionsNode->optionNodes[0] = &levelsNode;

  //mainMenuNode = &mainMenuNode;
}

void menuLoop(struct Node* startNode){
  JoystickOffset offset = userInputInit();
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

    if (currentNode == levelsNode){
      if(!lastButtonValue && joystickButton()){
        game_level_select(selectedOption);s
      };
        
        selectedOption = 0;
        OLED_buffer_clear();
      }

    }

    lastButtonValue = joystickButton();
    _delay_ms(50);
    printNodeUsingBuffer(currentNode, selectedOption);
    OLED_buffer_update_screen();
    /*if (currentNode->description == "Game"){
      send_joystick_position(offset);
    }*/
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

void game_level_select(uint8_t selected_option){
  struct CAN_msg msg;
  msg.id = 4;
  uint8_t array[8] = {selectedOption,0,0,0,0,0,0,0};
  for (int j = 0; j < 8; j++){
    msg.data[j] = array[j];
  }
  msg.length = 1;
  send_CAN_msg(&msg);
}

