#define F_CPU 5000000
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

//drivers
#include "../../drivers/include/OLEDDriver.h"
#include "../../drivers/include/CANDriver.h"
#include "../../drivers/include/ADCDriver.h"
#include "../include/gameMenu.h"
#include "../../drivers/include/userInputDriver.h"
#include "../../containers/include/gameStatusContainer.h"
#include <util/delay.h>

struct Node mainMenuNode;

struct Node playGameNode;
struct Node highScoresNode;

struct Node optionsNode;

struct Node usernameNode;

struct Node middleGameNode;
struct Node selectControllerNode;
struct Node endGameNode;
struct Node levelsNode;

uint8_t play_game;
uint8_t username = 0;
uint8_t restart_game;
uint16_t last_score = 0;
uint8_t select_game_controller;

struct Highscore highscore_data;

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
  playGameNode.description = "Game";
  playGameNode.numOptions = 0;

  highScoresNode.parent = &mainMenuNode;
  highScoresNode.options[0] = "Go back";
  highScoresNode.description = "Highscores TOP 3";
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

  levelsNode.parent = &usernameNode;
  levelsNode.options[0] = "Easy";
  levelsNode.options[1] = "Medium";
  levelsNode.options[2] = "Hard";
  levelsNode.options[3] = "Go back";
  levelsNode.description = "Select level";
  levelsNode.numOptions = 4;
  levelsNode.optionNodes[0] = &playGameNode;
  levelsNode.optionNodes[1] = &playGameNode;
  levelsNode.optionNodes[2] = &playGameNode;
  levelsNode.optionNodes[3] = &usernameNode;

  usernameNode.parent = &mainMenuNode;
  usernameNode.options[0] = "Magne";
  usernameNode.options[1] = "Andrine";
  usernameNode.options[2] = "Thea";
  usernameNode.options[3] = "Go back";
  usernameNode.description = "Who's playing?";
  usernameNode.numOptions = 4;
  usernameNode.optionNodes[0] = &levelsNode;
  usernameNode.optionNodes[1] = &levelsNode;
  usernameNode.optionNodes[2] = &levelsNode;
  usernameNode.optionNodes[3] = &mainMenuNode;

  highscore_data.users[0] = 3;
  highscore_data.users[1] = 3;
  highscore_data.users[2] = 3;

  highscore_data.scores[0] = 0;
  highscore_data.scores[1] = 0;
  highscore_data.scores[2] = 0;

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
        game_highscore_update();
        last_score = game_status_container_get_ptr()->score;
        play_game = 0;
        restart_game = 1;
        OLED_buffer_clear();
        game_status_container_init();
        currentNode = &endGameNode;
      }
      else if (game_status_container_get_ptr()->fail_detected){
        //Lost a life, need verification from user to restart the game
        last_score = game_status_container_get_ptr()->score;
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
        _delay_ms(50);
      }
    }
    else{
      //Inside the main menu system, game is not playing
      play_game = 0;

      //get joystick input
      JoystickCoords joystickCoords;
      joystickCoords = get_joystick_coords(ADC_ad_hoc_read(2),ADC_ad_hoc_read(1));
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

      //Checking if the user has selected a option
      if (!lastButtonValue && (get_slider_buttons() & 0x01)) {
        if (currentNode->description == "Who's playing?"){
          set_username(selectedOption);
        }
        if (currentNode->description == "Select game controller" && selectedOption!=2){
          select_game_controller = selectedOption;
        }
        if (currentNode->description == "Select level"){
          game_level_select(selectedOption);
        }
        if (currentNode->description == "Fail registered" &&
          currentNode->options[selectedOption] == "Back to main menu"){
          restart_game = 1;
        }
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
  OLED_buffer_print_line(node->description,0,0);
  if(node->description == "Highscores TOP 3"){

    for (int i = 1; i < 4; i++){
      print_highscore_node(i, highscore_data.users[i-1], (highscore_data.scores[i-1]));
    }
    for (int i = 0; i < node->numOptions; i++){
      if (i == selectedOption){
        OLED_buffer_print_line(node->options[i], i+6, 1);
      }
      else {
        OLED_buffer_print_line(node->options[i],i+6, 0);
      }
    }
  }
  else if(node->description == "Fail registered" || node->description == "All lives lost, game over"){

    print_score(2, last_score);
    for (int i = 0; i < node->numOptions; i++){
      if (i == selectedOption){
        OLED_buffer_print_line(node->options[i], i+4, 1);
      }
      else {
        OLED_buffer_print_line(node->options[i],i+4, 0);
      }
    }
  }
  else{

    for (int i = 0; i < node->numOptions; i++){
      if (i == selectedOption){
        OLED_buffer_print_line(node->options[i], i+2, 1);
      }
      else {
        OLED_buffer_print_line(node->options[i],i+2,0);
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
  cli();
  send_CAN_msg(&msg);
  sei();
}

void game_highscore_update(){
  //find highest score that's lower than new score
  for(uint8_t i = 1; i < 4; i++){
    //test if lower
    if(game_status_container_get_ptr()->score > highscore_data.scores[i-1]){

      //shift subsequent scores downward
      for(uint8_t j = 2; j >= i; j--){
        highscore_data.users[j] = highscore_data.users[j-1];
        highscore_data.scores[j] = highscore_data.scores[j-1];
      }

      //save the new score
      highscore_data.users[i-1] = username;
      highscore_data.scores[i-1] = game_status_container_get_ptr()->score;

      //your score has been saved in the highest possible position, finished
      return;
    }
  }
}


uint8_t get_play_game(){
  return play_game;
}

void set_play_game(uint8_t value){
  play_game = value;
}

void set_username(uint8_t name){
  username = name;
}

uint8_t get_restart_game(){
  return restart_game;
}

uint8_t get_game_select_controller(){
  return select_game_controller;
}
