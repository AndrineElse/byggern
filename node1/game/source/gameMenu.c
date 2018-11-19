#define F_CPU 5000000
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "../../drivers/include/CANDriver.h"
#include "../../drivers/include/ADCDriver.h"
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
struct Node levelsNode;

//struct GameData gameData;

uint8_t play_game;

void menuInit(){

  //printf("Inside init\n\r");

  playGameNode.parent = &mainMenuNode;
  playGameNode.description = "Game";
  playGameNode.numOptions = 0;
  playGameNode.optionNodes[0] = &mainMenuNode;

  highScoresNode.parent = &mainMenuNode;
  highScoresNode.options[0] = "Go back";
  highScoresNode.description = "Highscore";
  highScoresNode.numOptions = 1;
  highScoresNode.optionNodes[0] = &mainMenuNode;

  optionsNode.parent = &mainMenuNode;
  optionsNode.options[0] = "Go back";
  optionsNode.description = "Options";
  optionsNode.numOptions = 1;

  optionsNode.optionNodes[0] = &mainMenuNode;

  mainMenuNode.parent = (struct Node*)0;
  mainMenuNode.options[0] = "Play game";
  mainMenuNode.options[1] = "Highscores";
  mainMenuNode.options[2] = "Options";

  mainMenuNode.description = "This is the main menu :)";
  mainMenuNode.numOptions = 3;

  mainMenuNode.optionNodes[0] = &levelsNode;
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

  levelsNode.parent = &mainMenuNode;
  levelsNode.options[0] = "Easy";
  levelsNode.options[1] = "Medium";
  levelsNode.options[2] = "Hard";
  levelsNode.options[3] = "Go back";
  levelsNode.description = "Select level";
  levelsNode.numOptions = 4;
  levelsNode.optionNodes[0] = &playGameNode;
  levelsNode.optionNodes[1] = &playGameNode;
  levelsNode.optionNodes[2] = &playGameNode;
  levelsNode.optionNodes[3] = &mainMenuNode;

  //mainMenuNode = &mainMenuNode;
}

void menuLoop(){
  uint8_t selectedOption = 0;
  JoystickDir currentDir;
  volatile struct Node* currentNode = &mainMenuNode;
  JoystickDir lastDir;
  lastDir = 0;
  uint8_t lastButtonValue = 0;

  uint8_t numFails = 0;


  play_game = 0;
  /*
  gameData.gameStart = 0;
  gameData.pause = 0;
  gameData.calibrateEncoder = 0;
  */
  //volatile struct Game_status* game;
  while(1){
    //printf("Current node description: %s\n\r", currentNode->description );
    if(currentNode->description == "Game"){
        // printf("inside game node\n\r");
      //  printf("%d %d\n\r", game_status_container_get_ptr()->lives, game_status_container_get_ptr()->fails);

      if(game_status_container_get_ptr()->lives == game_status_container_get_ptr()->fails){
        //printf("GAME OVER\n\r");
        //gameData.gameStart = 0;
        //playGame = 1;

        play_game = 0;
        numFails = 0;
        currentNode = &endGameNode;
        // send_joystick_position(&joystick_timer,&send_joystick_flag, &playGame);
      }
      else if (numFails != game_status_container_get_ptr()->fails){
        //printf("inside fails node, numFails : %d, game fails: %d\n\r", numFails,game->fails);
        //gameData.gameStart = 0; //maybe pause? , set gameStart to initialize the game, and pause just pauses it???
        //playGame = 0;

        play_game = 0;
        numFails = game_status_container_get_ptr()->fails;
        currentNode = &middleGameNode;
        //send_joystick_position(&joystick_timer,&send_joystick_flag, &playGame);
      }
      else {
        play_game = 1;
        OLED_buffer_clear();
        OLED_buffer_update_screen();
        //send_joystick_position(&joystick_timer,&send_joystick_flag, &playGame);
        //_delay_ms(10);
        //gameData.gameStart = 1;
      }
    }
    else{
      //printf("Option nodes description: %s\n\r", currentNode->optionNodes[0]->description );
      //printf("inside menu looping\n\r");

      play_game = 0;
      //send_joystick_position(&joystick_timer,&send_joystick_flag, &playGame);

      //get joystick input
      cli();
      JoystickCoords joystickCoords;
      joystickCoords = get_joystick_coords(readChannel(2),readChannel(1));
      sei();
      JoystickDir currentDir;
      currentDir = calculate_joystick_dir(joystickCoords);
      

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

      if (!lastButtonValue && joystick_get_button() && currentNode->description == "Select level"){
          printf("%d\n\r", selectedOption);
          game_level_select(selectedOption);


        //selectedOption = 0;
        //OLED_buffer_clear();
      }

      if (!lastButtonValue && joystick_get_button()) {
        currentNode = currentNode->optionNodes[selectedOption];
        selectedOption = 0;
        OLED_buffer_clear();
      }
      //lastButtonValue = joystick_get_button();
      printNodeUsingBuffer(currentNode, selectedOption);
      OLED_buffer_update_screen();

      //_delay_ms(50);


      lastButtonValue = joystick_get_button();
      //_delay_ms(50);

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

void game_level_select(uint8_t selected_option){
  printf("A\n\r");
  struct CAN_msg msg;
  msg.id = 4;
  uint8_t array[8] = {selected_option,0,0,0,0,0,0,0};
  for (int j = 0; j < 8; j++){
    msg.data[j] = array[j];
  }
  msg.length = 1;
  cli();
  send_CAN_msg(&msg);
  sei();
}

uint8_t get_play_game(){
  return play_game;
}

void set_play_game(uint8_t value){
  play_game = value;
}
/*
  MAPPING
  id = 3
  data[0] = gameStart
  data[1] = pause
  data[2] = calibrateEncoder
  length = 3
*/
/*
void game_send_data_CAN(){
  // send can msg
  struct CAN_msg msg;
  msg.id = 4;
  uint8_t array[8] = {gameData.gameStart,gameData.pause,gameData.calibrateEncoder,0,0,0,0,0};
  for (int j = 0; j < 8; j++){
    msg.data[j] = array[j];

  }
  msg.length = 3;
  cli();
  send_CAN_msg(&msg);
  sei();
}*/
