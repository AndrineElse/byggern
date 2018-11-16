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
  input_container.game_flag = 0;
}

void input_container_update(struct CAN_msg new_input_message){
  input_container.joystick.x = new_input_message.data[0];
  input_container.joystick.y = new_input_message.data[1];
  input_container.joystickButton = (new_input_message.data[2] & 0x1);
  if(input_container.game_flag!=0){
      input_container.game_flag = new_input_message.id;
  }

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

void print_input_container(){
  printf("Input container X : %d\n\r",input_container.joystick.x );
  printf("Input container X : %d\n\r",input_container.joystick.y );
}

uint8_t input_container_message_received(){
  printf("Inside input container message recieved\n\r");
  if(input_container.game_flag!=0){
    return 1;
  }
  return 0;
}
