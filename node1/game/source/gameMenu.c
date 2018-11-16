#define F_CPU 5000000
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "../../drivers/include/CANDriver.h"
#include "../include/gameMenu.h"
#include "../../drivers/include/OLEDDriver.h"
#include "../../drivers/include/userInputDriver.h"
#include "../../containers/include/gameStatusContainer.h"
#include <util/delay.h>

struct Node mainMenuNode;

struct Node playGameNode;
struct Node highScoresNode;

struct Node optionsNode;

struct Node middleGameNode;

struct Node endGameNode;

struct GameData gameData;

void menuInit(){

  //printf("Inside init\n\r");

  playGameNode.parent = &mainMenuNode;
  playGameNode.options[0] = "Go back";
  playGameNode.description = "Game";
  playGameNode.numOptions = 1;
  playGameNode.optionNodes[0] = &mainMenuNode;

  highScoresNode.parent = &mainMenuNode;
  highScoresNode.options[0] = "Go back";
  highScoresNode.description = "highscore";
  highScoresNode.numOptions = 1;
  highScoresNode.optionNodes[0] = &mainMenuNode;

  optionsNode.parent = &mainMenuNode;
  optionsNode.options[0] = "Go back";
  optionsNode.description = "options";
  optionsNode.numOptions = 1;
  optionsNode.optionNodes[0] = &mainMenuNode;

  mainMenuNode.parent = (struct Node*)0;
  mainMenuNode.options[0] = "Play game";
  mainMenuNode.options[1] = "Highscores";
  mainMenuNode.options[2] = "Options";

  mainMenuNode.description = "This is the main menu :)";
  mainMenuNode.numOptions = 3;

  mainMenuNode.optionNodes[0] = &playGameNode;
  mainMenuNode.optionNodes[1] = &highScoresNode;
  mainMenuNode.optionNodes[2] = &optionsNode;

  endGameNode.parent = (struct Node*)0;
  endGameNode.description = "All lives lost, game over";
  endGameNode.numOptions = 2;
  endGameNode.options[0] = "New game";
  endGameNode.options[1] = "Back to main menu";
  endGameNode.optionNodes[0] = &playGameNode;
  endGameNode.optionNodes[1] = &mainMenuNode;
  //mainMenuNode = &mainMenuNode;

  middleGameNode.parent = (struct Node*)0;
  middleGameNode.description = "Fail registerd";
  middleGameNode.numOptions = 2;
  middleGameNode.options[0] = "Continue game";
  middleGameNode.options[1] = "Back to main menu";
  middleGameNode.optionNodes[0] = &playGameNode;
  middleGameNode.optionNodes[1] = &mainMenuNode;

  //mainMenuNode = &mainMenuNode;
}

void menuLoop(){
  JoystickOffset offset = userInputInit();
  uint8_t selectedOption = 0;
  JoystickDir currentDir;
  volatile struct Node* currentNode = &mainMenuNode;
  JoystickDir lastDir;
  lastDir = calculateJoystickDirection();
  uint8_t lastButtonValue = 0;
  uint8_t gameFlag = 1;
  uint8_t numFails = 0;

  gameData.gameStart = 0;
  gameData.pause = 0;
  gameData.calibrateEncoder = 0;
  //volatile struct Game_status* game;
  while(1){
    //printf("Current node description: %s\n\r", currentNode->description );
    if(currentNode->description == "Game"){
        // printf("inside game node\n\r");
      if(gameFlag){
        game_send_data_CAN();
        OLED_clear();
        send_joystick_position(offset);
        gameData.gameStart = 1;

      }
      if (numFails != game_status_container_get_ptr()->fails){
        //printf("inside fails node, numFails : %d, game fails: %d\n\r", numFails,game->fails);
        gameData.gameStart = 0; //maybe pause? , set gameStart to initialize the game, and pause just pauses it???
        gameFlag = 0;
        numFails = game_status_container_get_ptr()->fails;
        currentNode = &middleGameNode;
      }
      else if(game_status_container_get_ptr()->lives == game_status_container_get_ptr()->fails){
        //printf("GAME OVER\n\r");
        gameData.gameStart = 0;
        gameFlag = 0;
        numFails = 0;
        currentNode = &endGameNode;
      }
    }
    else{
      //printf("Option nodes description: %s\n\r", currentNode->optionNodes[0]->description );
      //printf("inside menu looping\n\r");
      gameFlag = 1;
      //get joystick input
      JoystickCoords joystickCoords;
      joystickCoords = calculateCalibratedJoystickCoords(offset);
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
        lastDir = currentDir;
      }
      if (!lastButtonValue && joystickButton()) {
        currentNode = currentNode->optionNodes[selectedOption];
        selectedOption = 0;
        OLED_buffer_clear();
      }

      lastButtonValue = joystickButton();
      _delay_ms(50);
      printNodeUsingBuffer(currentNode, selectedOption);
      OLED_buffer_update_screen();
    }
  }
}

void printNode(volatile struct Node* node, uint8_t selectedOption){
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

void printNodeUsingBuffer(volatile struct Node* node, uint8_t selectedOption){

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


/*
  MAPPING
  id = 3
  data[0] = gameStart
  data[1] = pause
  data[2] = calibrateEncoder
  length = 3
*/

void game_send_data_CAN(){
  // send can msg
  struct CAN_msg msg;
  msg.id = 3;
  uint8_t array[8] = {gameData.gameStart,gameData.pause,gameData.calibrateEncoder,0,0,0,0,0};
  for (int j = 0; j < 8; j++){
    msg.data[j] = array[j];

  }
  msg.length = 3;
  send_CAN_msg(&msg);
}
