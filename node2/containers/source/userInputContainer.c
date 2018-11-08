#include <stdint.h>
#include "../include/userInputContainer.h"
#include "../../drivers/include/CANDriver.h"

volatile struct Input_status input_container;

void User_input_init() {
  // setting reasonable initial input values
  input_container.joystick.x = 0;
  input_container.joystick.y = 0;
  input_container.joystickButton = 0;
  input_container.left_slider = 0; // not in use
  input_container.right_slider = 0; // not in use
  input_container.right_button = 0; // not in use
  input_container.left_button = 0; // not in use
}

void input_update(struct CAN_msg new_input_message){
  input_container.joystick.x = new_input_message.data[0];
  input_container.joystick.y = new_input_message.data[1];
  input_container.joystickButton = (new_input_message.data[2] & 0x1);
  /*
  input_container.left_slider = 0; // not in use
  input_container.right_slider = 0; // not in use
  input_container.right_button = 0; // not in use
  input_container.left_button = 0; // not in use
  */
}

volatile struct Input_status* get_input_ptr() {
  return &input_container;
}