#include <stdint.h>
#include <stdio.h>
#include "../../drivers/include/CANDriver2.h"
#include "../include/gameDataContainer.h"

volatile struct GameData game_data_container;

void game_data_container_init(){
  game_data_container.gameStart = 0;
  game_data_container.pause = 0;
  game_data_container.calibrateEncoder = 0;
}

void game_data_container_update(struct CAN_msg new_game_message){
  game_data_container.gameStart = new_game_message.data[0];
  game_data_container.pause = new_game_message.data[1];
  game_data_container.calibrateEncoder = new_game_message.data[2];

}

volatile struct GameData* game_data_container_get_ptr(){
  return &game_data_container;
}

void print_game_data_container(){
  //printf("Lives : %d Fails: %d \n", game_status_container.lives,game_status_container.fails );
}
