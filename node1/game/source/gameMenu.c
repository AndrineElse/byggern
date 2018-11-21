#define F_CPU 5000000
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>

//drivers
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
struct Node selectControllerNode;
struct Node middleGameNode;

struct Node endGameNode;
struct Node levelsNode;



uint8_t play_game;
uint8_t restart_game;
uint8_t select_game_controller;

void menuInit(){

  //Initializing main menu system nodes

  playGameNode.parent = &mainMenuNode;
  playGameNode.description = "Game";
  playGameNode.numOptions = 0;
  playGameNode.optionNodes[0] = &mainMenuNode;

  highScoresNode.parent = &mainMenuNode;
  highScoresNode.options[0] = "Go back";
  highScoresNode.description = "Highscore";
  highScoresNode.numOptions = 1;
  highScoresNode.optionNodes[0] = &mainMenuNode;

  selectControllerNode.parent = &optionsNode;
  selectControllerNode.options[0] = "Joystick";
  selectControllerNode.options[1] = "Slider right";
  selectControllerNode.options[2] = "Go back";
  selectControllerNode.description = "Select game controller";
  selectControllerNode.numOptions = 3;
  selectControllerNode.optionNodes[0] = &mainMenuNode;
  selectControllerNode.optionNodes[1] = &mainMenuNode;
  selectControllerNode.optionNodes[2] = &optionsNode;


  optionsNode.parent = &mainMenuNode;
  optionsNode.options[0] = "Select game controller";
  optionsNode.options[1] = "Go back";
  optionsNode.description = "Options";
  optionsNode.numOptions = 2;
  optionsNode.optionNodes[1] = &mainMenuNode;
  optionsNode.optionNodes[0] = &selectControllerNode;


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


  middleGameNode.parent = (struct Node*)0;
  middleGameNode.description = "Fail registered";
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
}

void menuLoop(){
  uint8_t selectedOption = 0;
  JoystickDir currentDir;
  volatile struct Node* currentNode = &mainMenuNode;
  JoystickDir lastDir = 0;
  uint8_t lastButtonValue = 0;
  restart_game = 0;
  select_game_controller = 1;
  while(1){
    if(currentNode->description == "Game"){
      if(game_status_container_get_ptr()->lives == game_status_container_get_ptr()->fails){
        //All lives are lost, game over.
        play_game = 0;
        restart_game = 1;
        OLED_buffer_clear();
        game_status_container_init();
        currentNode = &endGameNode;
      }
      else if (game_status_container_get_ptr()->fail_detected){
        //Lost a life, need verification from user to restart the game
        OLED_buffer_clear();
        play_game = 0;
        currentNode = &middleGameNode;
      }
      else {
        //Playing game, sending
        OLED_buffer_clear();
        OLED_fun();
        restart_game = 0;
        play_game = 1;
        //OLED_buffer_clear();
        OLED_buffer_update_screen();
      }
    }
    else{
      //Inside the main menu system, game is not playing
      play_game = 0;

      //get joystick input
      cli();
      JoystickCoords joystickCoords;
      joystickCoords = get_joystick_coords(readChannel(2),readChannel(1));
      sei();
      JoystickDir currentDir;
      currentDir = calculate_joystick_dir(joystickCoords);

      //Finding index for selected option
      if (currentDir != lastDir){
        switch (currentDir) {
          case UP:
            if (selectedOption > 0){
              selectedOption = selectedOption -1;
            }
            break;
          case DOWN:
            if (selectedOption < (currentNode->numOptions-1)){
              selectedOption = selectedOption +1;
            }
            break;
        }
        lastDir = currentDir;
      }

      if (!lastButtonValue && (get_slider_buttons() & 0x01) && currentNode->description == "Select level"){
          game_level_select(selectedOption);
      }

      if (!lastButtonValue && (get_slider_buttons() & 0x01) && currentNode->description == "Select game controller"){
          if(selectedOption!=2){
            select_game_controller = selectedOption;
          }
      }

      //Checking if the user has selected a option
      if (!lastButtonValue && (get_slider_buttons() & 0x01)) {
        currentNode = currentNode->optionNodes[selectedOption];
        selectedOption = 0;
        OLED_buffer_clear();
      }
      lastButtonValue = (get_slider_buttons() & 0x01);

      //printing the current node info to the OLED
      printNodeUsingBuffer(currentNode, selectedOption);
      OLED_buffer_update_screen();

    }
  }
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

uint8_t get_restart_game(){
  return restart_game;
}

uint8_t get_game_select_controller(){
  return select_game_controller;
}
