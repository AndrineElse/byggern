#define F_CPU 5000000
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

//#include "../../fonts.h"

//drivers
#include "../../drivers/include/OLEDDriver.h"
#include "../../drivers/include/CANDriver.h"
#include "../include/gameMenu.h"
#include "../../drivers/include/userInputDriver.h"
#include "../../containers/include/gameStatusContainer.h"
#include "../../drivers/include/ADCDriver.h"

struct Node mainMenuNode;
struct Node playGameNode;
struct Node highScoresNode;
struct Node optionsNode;
struct Node levelsNode;
struct Node usernameNode;

struct Node middleGameNode;
struct Node endGameNode;

uint8_t play_game;
uint8_t username = 0;
uint16_t score_array[3] = {0,0,0};
uint8_t restart_game;

void menuInit(){

  //Initializing main menu system nodes

  mainMenuNode.parent = (struct Node*)0;
  mainMenuNode.options[0] = "Play game";
  mainMenuNode.options[1] = "Highscores";
  mainMenuNode.options[2] = "Options";
  mainMenuNode.description = "This is the main menu :)";
  mainMenuNode.numOptions = 3;
  mainMenuNode.optionNodes[0] = &usernameNode;
  mainMenuNode.optionNodes[1] = &highScoresNode;
  mainMenuNode.optionNodes[2] = &optionsNode;


  playGameNode.parent = &usernameNode;
  //playGameNode.options[0] = "Go back";
  playGameNode.description = "Game";
  playGameNode.numOptions = 0;
  //playGameNode.optionNodes[0] = &mainMenuNode;

  highScoresNode.parent = &mainMenuNode;
  /*highScoresNode.options[0] = "1. ";
  highScoresNode.options[1] = "2. ";
  highScoresNode.options[2] = "3. ";*/
  highScoresNode.options[0] = "Go back";
  highScoresNode.description = "Highscores TOP 3";
  highScoresNode.numOptions = 1;
  highScoresNode.optionNodes[0] = &mainMenuNode;

  optionsNode.parent = &mainMenuNode;
  optionsNode.options[0] = "Select level";
  optionsNode.options[1] = "Go back";
  optionsNode.description = "Options";
  optionsNode.numOptions = 2;
  optionsNode.optionNodes[0] = &levelsNode;
  optionsNode.optionNodes[1] = &mainMenuNode;

  middleGameNode.parent = (struct Node*)0;
  middleGameNode.description = "Fail registerd";
  middleGameNode.numOptions = 3;
  middleGameNode.options[0] = "Your score: ";// game_status_container_get_ptr()->score;
  middleGameNode.options[1] = "Continue game";
  middleGameNode.options[2] = "Back to main menu";
  middleGameNode.optionNodes[0] = &mainMenuNode;
  middleGameNode.optionNodes[1] = &playGameNode;
  middleGameNode.optionNodes[2] = &mainMenuNode;

  endGameNode.parent = (struct Node*)0;
  endGameNode.description = "All lives lost, game over";
  endGameNode.numOptions = 3;
  endGameNode.options[0] = "Your score: ";// game_status_container_get_ptr()->score;
  endGameNode.options[1] = "New game";
  endGameNode.options[2] = "Back to main menu";
  endGameNode.optionNodes[0] = &mainMenuNode;
  endGameNode.optionNodes[1] = &playGameNode;
  endGameNode.optionNodes[2] = &mainMenuNode;

  levelsNode.parent = &optionsNode;
  levelsNode.options[0] = "Easy";
  levelsNode.options[1] = "Medium";
  levelsNode.options[2] = "Hard";
  levelsNode.options[3] = "Go back";
  levelsNode.description = "Select level";
  levelsNode.numOptions = 4;
  levelsNode.optionNodes[0] = &optionsNode;
  levelsNode.optionNodes[1] = &optionsNode;
  levelsNode.optionNodes[2] = &optionsNode;
  levelsNode.optionNodes[3] = &optionsNode;

  usernameNode.parent = &mainMenuNode;
  usernameNode.options[0] = "Magne";
  usernameNode.options[1] = "Andrine";
  usernameNode.options[2] = "Thea";
  usernameNode.description = "Who's playing?";
  usernameNode.numOptions = 3;
  usernameNode.optionNodes[0] = &playGameNode;
  usernameNode.optionNodes[1] = &playGameNode;
  usernameNode.optionNodes[2] = &playGameNode;
}

void menuLoop(){
  uint8_t selectedOption = 0;
  JoystickDir currentDir;
  volatile struct Node* currentNode = &mainMenuNode;
  JoystickDir lastDir = 0;
  uint8_t lastButtonValue = 0;
  restart_game = 0;
  while(1){
    if(currentNode->description == "Game"){
      if(game_status_container_get_ptr()->lives == game_status_container_get_ptr()->fails){
        //All lives are lost, game over.
        play_game = 0;
        restart_game = 1;
        game_status_container_init();
        currentNode = &endGameNode;
      }
      else if (game_status_container_get_ptr()->fail_detected){
        //Lost a life, need verification from user to restart the game
        play_game = 0;
        currentNode = &middleGameNode;
      }
      else {
        //Playing game, sending
        restart_game = 0;
        play_game = 1;
        OLED_buffer_clear();
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

      if (!lastButtonValue && joystick_get_button() && currentNode->description == "Select level"){
          //printf("%d\n\r", selectedOption);
          game_level_select(selectedOption);
      }

      if (!lastButtonValue && joystick_get_button() && currentNode->description == "Who's playing?"){
          game_username_select(selectedOption);
      }
      /*if (!lastButtonValue && joystick_get_button() && currentNode->description == "Highscores: TOP 3"){
          print_highscore_node(char place, uint8_t username, uint16_t score);
      }*/


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
    //_delay_ms(50);
    }

  }
}


void printNodeUsingBuffer(volatile struct Node* node, uint8_t selectedOption){
  OLED_buffer_print_line(node->description,0,0);
  if(node->description == "Highscores TOP 3"){


    print_highscore_place(3, 1, 1);
    //print_highscore_place(2, 2);
    //print_highscore_place(3, 3);
    //print_highscore_node(1, game_highscore_update()[1], 0, 0);
    for (int i = 0; i < node->numOptions; i++){
      if (i == selectedOption){
        OLED_buffer_print_line(node->options[i], i+4, 1);
      }
      else {
        OLED_buffer_print_line(node->options[i],i+4,0);
      }
    }
  }
  else{
    //OLED_buffer_print_line(node->description,0,0);

    for (int i = 0; i < node->numOptions; i++){
      if (i == selectedOption){
        OLED_buffer_print_line(node->options[i], i+1, 1);
      }
      else {
        OLED_buffer_print_line(node->options[i],i+1,0);
      }
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
}

uint8_t* game_highscore_update(){

  uint8_t highscore_data[4];
  for (int i = 0; i < 3; i++){
    if (highScoresNode.options[i] == "-"|| game_status_container_get_ptr()->score > score_array[i]){
      highscore_data[0] = i;
      highscore_data[1] = username;
      highscore_data[2] = (game_status_container_get_ptr()->score >> 8) & 0xFF;
      highscore_data[3] = game_status_container_get_ptr()->score & 0xFF;

      score_array[i] = game_status_container_get_ptr()->score;
      // game_highscore_SRAM_update(game_status_container_get_ptr()->user, game_status_container_get_ptr()->score);
      // game_highscore_SRAM_get(uint8_t place)
      //highScoresNode.options[i] = username; // + ': ' + (char*)(game_status_container_get_ptr()->score);


    }
    else{
      highscore_data[0] = i;
      highscore_data[1] = username;
      highscore_data[2] = 0;
      highscore_data[3] = 0;
    }
  }
  return highscore_data;
}


uint8_t get_play_game(){
  return play_game;
}

void set_play_game(uint8_t value){
  play_game = value;
}

uint8_t get_username(){
  return username;
}

void set_username(uint8_t name){
  username = name;
}

void game_username_select(uint8_t selectedOption){
  // game_user_update(node->options[selectedOption]);
  set_username(selectedOption);
}

uint8_t get_restart_game(){
  return restart_game;
}
