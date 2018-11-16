#define F_CPU 5000000
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>
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

/*void menuLoop(){
  //printf("inside menu loop\n\r");



  //printf("Inside menu function\n\r");
  JoystickOffset offset = userInputInit();
  uint8_t selectedOption = 0;
  JoystickDir currentDir;
  volatile struct Node* currentNode = &mainMenuNode;
  JoystickDir lastDir;
  lastDir = calculateJoystickDirection();
  uint8_t lastButtonValue = 0;
  JoystickOffset joystickOffset;
  joystickOffset = calculateOffsetJoystick();
  JoystickCoords joystickCoords;
  printf("b");
  while(1){
      printf("a");
    //printf("Inside menu loop\n\r");
    //get joystick input

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
    cli();
    printNodeUsingBuffer(currentNode, selectedOption);
    OLED_buffer_update_screen();
    sei();
  }

}*/

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
  //volatile struct Game_status* game;
  while(1){
    //printf("Current node description: %s\n\r", currentNode->description );
    if(currentNode->description == "Game"){
        // printf("inside game node\n\r");
      if(gameFlag){
        OLED_clear();
        //printf("inside playing game\n\r");
        send_joystick_position(offset);
        //printf("joystick pos sent\n\r");
        //game = game_status_container_get_ptr();
      }
      if (numFails != game_status_container_get_ptr()->fails){
        //printf("inside fails node, numFails : %d, game fails: %d\n\r", numFails,game->fails);
        gameFlag = 0;
        numFails = game_status_container_get_ptr()->fails;
        currentNode = &middleGameNode;
      }
      else if(game_status_container_get_ptr()->lives == game_status_container_get_ptr()->fails){
        //printf("GAME OVER\n\r");
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
