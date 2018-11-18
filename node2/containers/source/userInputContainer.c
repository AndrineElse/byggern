#include <stdint.h>
#include "../../drivers/include/CANDriver2.h"
#include "../include/userInputContainer.h"

volatile struct Input_status input_container;

void input_container_init() {
  // setting reasonable initial input values
  input_container.joystick.x = 0;
  input_container.joystick.y = 0;
  input_container.joystickButton = 0;
  input_container.left_slider = 0; // not in use
  input_container.right_slider = 0; // not in use
  input_container.right_button = 0; // not in use
  input_container.left_button = 0; // not in use
  input_container.playGame = 0;
}

void input_container_update(struct CAN_msg new_input_message){

  input_container.joystick.x = new_input_message.data[0];
  input_container.joystick.y = new_input_message.data[1];
  input_container.joystickButton = (new_input_message.data[2] & 0x01);
  input_container.playGame = ((new_input_message.data[2] & 0x2)>>1);
  printf("%x %x\n\r", input_container.playGame, input_container.joystickButton);
  /*
  input_container.left_slider = 0; // not in use
  input_container.right_slider = 0; // not in use
  input_container.right_button = 0; // not in use
  input_container.left_button = 0; // not in use
  */
}

volatile struct Input_status* input_container_get_ptr() {
  return &input_container;
}
