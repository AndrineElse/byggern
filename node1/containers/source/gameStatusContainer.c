#include <stdint.h>
#include <stdio.h>
#include "../../drivers/include/CANDriver.h"
#include "../include/gameStatusContainer.h"

volatile struct Game_status game_status_container;

void game_status_container_init(){
  game_status_container.fails = 0;
  game_status_container.timer = 0;
  game_status_container.lives = 3;
  game_status_container.score = 0;
  game_status_container.playing = 0; //???
  game_status_container.fail_detected = 0;
}

void game_status_container_update(struct CAN_msg new_game_message){
  game_status_container.fails = ((new_game_message.data[0] & 0x38)>>3); //gets bit 4,5,6
  game_status_container.timer = new_game_message.data[0];
  game_status_container.lives = (new_game_message.data[0] & 0x07); //gets bit 1,2,3
  game_status_container.score = new_game_message.data[3];
  game_status_container.fail_detected = ((new_game_message.data[0] & 0x40)>>6);
}

volatile struct Game_status* game_status_container_get_ptr(){
  return &game_status_container;
}
