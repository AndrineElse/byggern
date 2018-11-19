#define F_CPU 5000000
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//drivers
#include "../../drivers/include/CANDriver.h"
#include "../include/gameMenu.h"
#include "../../drivers/include/OLEDDriver.h"
#include "../../drivers/include/userInputDriver.h"
#include "../../containers/include/gameStatusContainer.h"
#include "../../drivers/include/ADCDriver.h"

struct Node mainMenuNode;
struct Node playGameNode;
struct Node highScoresNode;
struct Node optionsNode;
struct Node middleGameNode;
struct Node endGameNode;
struct Node watchReplayNode;


//global variables to indicate desired state to node2 via CAN
uint8_t play_game;
uint8_t restart_game;
uint8_t run_playback;

void menuInit(){

  //Initializing main menu system nodes

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
  endGameNode.numOptions = 3;
  endGameNode.options[0] = "New game";
  endGameNode.options[1] = "Back to main menu";
  endGameNode.options[2] = "Watch replay";
  endGameNode.optionNodes[0] = &playGameNode;
  endGameNode.optionNodes[1] = &mainMenuNode;
  endGameNode.optionNodes[2] = &watchReplayNode;


  middleGameNode.parent = (struct Node*)0;
  middleGameNode.description = "Fail registered";
  middleGameNode.numOptions = 3;
  middleGameNode.options[0] = "Continue game";
  middleGameNode.options[1] = "Back to main menu";
  middleGameNode.options[2] = "Watch replay";
  middleGameNode.optionNodes[0] = &playGameNode;
  middleGameNode.optionNodes[1] = &mainMenuNode;
  middleGameNode.optionNodes[2] = &watchReplayNode;

  watchReplayNode.parent = (struct Node*)0;
  watchReplayNode.description = "Watching replay";
  watchReplayNode.numOptions = 1;
  watchReplayNode.options[0] = "Stop watching";
  watchReplayNode.optionNodes[0] = &mainMenuNode;
  //optionnode pointer has to be set to whatever state you started watching from,
  //either middleGameNode or endGameNode.

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

      if( (currentNode->description == "Watching replay") &&
          (!game_status_container_get_ptr()->running_playback)){
          currentNode = currentNode->optionNodes[0];
          run_playback = 0;
      }
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
      //Checking if the user has selected a option
      if (!lastButtonValue && (get_slider_buttons() & 0x01)) {
        printf("push!\n\r");
        //add specific handling for node transistions here
        if(currentNode->optionNodes[selectedOption]->description == "Watching replay"){
          printf("playback\n\r");
          watchReplayNode.optionNodes[0] = currentNode;
          run_playback = 1;
          while(1){
            printf("w:%d\n\r",get_run_playback());
          }
          //hold the program here until node2 acknowledges playback
          while(!game_status_container_get_ptr()->running_playback) {
            printf("waiting\n\r");
          }

        } else if (currentNode->description == "Watching replay") {
          run_playback = 0;
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

uint8_t get_play_game(){
  return play_game;
}

void set_play_game(uint8_t value){
  play_game = value;
}

uint8_t get_restart_game(){
  return restart_game;
}


uint8_t get_run_playback() {
  return run_playback;
}
