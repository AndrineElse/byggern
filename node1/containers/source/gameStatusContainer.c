#include <stdint.h>
#include "../../drivers/include/CANDriver2.h"
#include "../include/userInputContainer.h"

volatile struct Game_status game_status_container;

void game_status_container_init(){
  game_status_container.fails = 0;
  game_status_container.timer = 0;
  game_status_container.lives = 3;
  game_status_container.score = 0;
}

void game_status_container_update(struct CAN_msg new_game_message){
  game_status_container.fails = new_game_message.data[0];
  game_status_container.timer = new_game_message.data[1];
  game_status_container.lives = new_game_message.data[2];
  game_status_container.score = new_game_message.data[3];
}

volatile struct Game_status* game_status_container_get_ptr(){
  return &game_status_container;
}
