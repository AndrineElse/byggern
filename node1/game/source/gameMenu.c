#include <stdint.h>
#include "../../drivers/include/CANDriver.h"
#include "../../containers/include/gameStatusContainer.h"
#include "../include/gameMenu.h"
#include "../../drivers/include/OLEDDriver.h"
#include "../../drivers/include/userInputDriver.h"
#include <util/delay.h>

struct Node mainMenuNode;
struct Node playGameNode;
struct Node highScoresNode;
struct Node optionsNode;
struct Node endGameNode;
struct Node middleGameNode;

void menuInit(){

  //struct Node mainMenuNode;
  struct Node playGameNode;
  playGameNode.parent = &mainMenuNode;
  playGameNode.options[0] = "Go back";
  playGameNode.description = "Game";
  playGameNode.numOptions = 1;

  struct Node highScoresNode;
  highScoresNode.parent = &mainMenuNode;
  highScoresNode.options[0] = "Go back";
  highScoresNode.description = "highscore";
  highScoresNode.numOptions = 1;

  struct Node optionsNode;
  optionsNode.parent = &mainMenuNode;
  optionsNode.options[0] = "Go back";
  optionsNode.description = "options";
  optionsNode.numOptions = 1;


  mainMenuNode.parent = (struct Node*)0;
  mainMenuNode.options[0] = "Play game";
  mainMenuNode.options[1] = "Highscores";
  mainMenuNode.options[2] = "Options";

  mainMenuNode.description = "This is the main menu :)";
  mainMenuNode.numOptions = 3;

  mainMenuNode.optionNodes[0] = &playGameNode;
  mainMenuNode.optionNodes[1] = &highScoresNode;
  mainMenuNode.optionNodes[2] = &optionsNode;

  //mainMenuNode = &mainMenuNode;
}

void menuLoop(){
  JoystickOffset offset = userInputInit();
  uint8_t selectedOption = 0;
  JoystickDir currentDir;
  struct Node* currentNode = startNode;
  JoystickDir lastDir;
  lastDir = calculateJoystickDirection();
  uint8_t lastButtonValue = 0;
  uint8_t gameFlag = 1;
  uint8_t numFails= 0;
  volatile struct Game_status* game;
  while(1){
    //printf("Current node description: %s\n\r", currentNode->description );
    if(currentNode->description == "Game"){
        printf("inside game node\n\r");
      if(gameFlag){
        printf("inside playing game\n\r");
        send_joystick_position(offset);
        printf("joystick pos sent\n\r");
        game = game_status_container_get_ptr();
      }
      if (numFails != game->fails){
        printf("inside fails node, numFails : %d, game fails: %d\n\r", numFails,game->fails);
        gameFlag = 0;
        numFails = game->fails;
        currentNode = getMiddleGameNode(startNode);
      }
      else if(game->lives == game->fails){
        printf("Failed game\n\r");
        gameFlag = 0;
        numFails = 0;
        currentNode = getEndGameNode(startNode);
      }
    }
    else{
      printf("Option nodes description: %s\n\r", currentNode->optionNodes[0]->description );
      printf("inside menu looping\n\r");
      gameFlag = 1;
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

struct Node* getEndGameNode(struct Node* startNode){
  struct Node endGameNode;
  struct Node playGameNode;
  playGameNode.parent = startNode;
  playGameNode.options[0] = "Go back";
  playGameNode.description = "Game";
  playGameNode.numOptions = 1;

  endGameNode.parent = (struct Node*)0;
  endGameNode.description = "All lives lost, game ended!";
  endGameNode.numOptions = 2;
  endGameNode.options[0] = "New game";
  endGameNode.options[1] = "Back to main menu";
  endGameNode.optionNodes[0] = &playGameNode;
  endGameNode.optionNodes[1] = startNode;

  return &endGameNode;
}

struct Node* getMiddleGameNode(struct Node* startNode){
  struct Node middleGameNode;
  struct Node playGameNode;
  playGameNode.parent = startNode;
  playGameNode.options[0] = "Go back";
  playGameNode.description = "Game";
  playGameNode.numOptions = 1;

  middleGameNode.parent = (struct Node*)0;
  middleGameNode.description = "Failed registerd!";
  middleGameNode.numOptions = 2;
  middleGameNode.options[0] = "Continue game";
  middleGameNode.options[1] = "Back to main menu";
  middleGameNode.optionNodes[0] = &playGameNode;
  middleGameNode.optionNodes[1] = startNode;

  return &middleGameNode;
}
