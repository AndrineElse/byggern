#include <stdint.h>
#include "../../drivers/include/CANDriver2.h"
#include "../include/userInputContainer.h"

volatile struct Input_status input_container;

void input_container_init() {
  // setting reasonable initial input values
  input_container.joystick.x = 0;
  input_container.joystick.y = 0;
  input_container.joystickButton = 0;
  input_container.right_slider = 0;
  input_container.playGame = 0;
  input_container.restart_game = 1;
  input_container.select_game_controller = 0;
}

void input_container_update(struct CAN_msg new_input_message){
  input_container.select_game_controller = ((new_input_message.data[2] & 0x8) >> 3);
  if(input_container.select_game_controller){
    input_container.right_slider = new_input_message.data[1];
  }
  else{
    input_container.joystick.y = new_input_message.data[1];
  }
  input_container.joystick.x = new_input_message.data[0];
  input_container.joystickButton = (new_input_message.data[2] & 0x01);
  input_container.playGame = (( new_input_message.data[2] & 0x2 ) >> 1);
  input_container.restart_game = ((new_input_message.data[2] & 0x4) >> 2);
}

volatile struct Input_status* input_container_get_ptr() {
  return &input_container;
}
