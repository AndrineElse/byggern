#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "../include/ADC_driver.h"
#include "../include/user_input_driver.h"
#include "../include/CAN_driver.h"

Joystick_offset user_input_init(){
  Joystick_offset offset = get_offset_joystick();
  return offset;
}

Joystick_coords get_calibrated_joystick_coords(Joystick_offset offset) {
  uint8_t rawX = ADC_read_channel(2);
  uint8_t rawY = ADC_read_channel(1);

  int16_t xValue = ((int16_t)rawX) - offset.x;
  int16_t yValue = ((int16_t)rawY) - offset.y;

  //saturation in offset, maintains strict interval [0,255]
  xValue = (xValue > 255) ? 255 : xValue;
  xValue = (xValue < 0) ? 0 : xValue;
  yValue = (yValue > 255) ? 255 : yValue;
  yValue = (yValue < 0) ? 0 : yValue;

  //Completely right should be 255, completely left should be 0
  Joystick_coords sampledValues;
  sampledValues.x = get_joystick_mapping(xValue);
  sampledValues.y = get_joystick_mapping(yValue);

  return sampledValues;
}

int8_t get_joystick_mapping(int16_t raw_value) {
  return (int8_t)((raw_value-128 )/(128/100.0));
}

Joystick_offset get_offset_joystick() {
  //optimization option: send address once, sample multiple times.
  //performance option: more samples, median of samples..

  //Sample a couple of times, do average, set as offset value
  int8_t totX = 0;
  int8_t totY = 0;
  for(int i = 0; i < 3; i++) {
    totX += (int16_t)ADC_read_channel(2) - 128;
    totY += (int16_t)ADC_read_channel(1) - 128;
  }
  Joystick_offset finalOffset;
  final_offset.x = totX/3;
  final_offset.y = totY/3;

  return final_offset;
}

Joystick_dir get_joystick_direction(Joystick_coords coords){
  if ((coords.x == coords.y) || (abs(abs(coords.x)-abs(coords.y)) < 20)){
    return NEUTRAL;
  }
  else if( coords.x > 83){
    return RIGHT;
  }
  else if (coords.x < -83){
    return LEFT;
  }
  else if(coords.y > 83){
    return UP;
  }
  else if(coords.y < -83){
    return DOWN;
  }
  return NEUTRAL;
}


Slider_position get_slider_position(){

  Slider_position position;
  uint8_t rawLeft = ADC_read_channel(3);
  uint8_t rawRight = ADC_read_channel(4);

  position.left = rawLeft;
  position.right = rawRight;

  return position;
}

uint8_t get_joystick_button(){
  return !(PINB & 0x01);
}

Joystick_dir get_current_joystickDir(){
  Joystick_offset joystickOffset;
  joystickOffset = get_offset_joystick();
  Joystick_coords joystickCoords;
  joystickCoords = get_calibrated_joystick_coords(joystickOffset);
  Joystick_dir joystickDir;
  joystickDir = get_joystick_direction(joystickCoords);
  return joystickDir;
}


//MAPPING
// id for joystick = 1
// data[0] = x
// data[1] = y
// data[2] = button (LSB = button), 7 unused bits here


void send_joystick_position_CAN(Joystick_offset offset){
  //sends joystick position from node 1 to node 2
  struct CAN_msg msg;
  struct Joystick_coords coords;
  coords = get_calibrated_joystick_coords(offset);
  msg.id = 1;
  uint8_t array[8] = {coords.x,coords.y,get_joystick_button(),0,0,0,0,0};

  for (int j = 0; j < 8; j++){
    msg.data[j] = array[j];

  }
  msg.length = 3;
  CAN_send_msg(&msg);
  printf("CoordsX: %d, CoordsY: %d, Button: %d\n\r", coords.x, coords.y, get_joystick_button());

}
