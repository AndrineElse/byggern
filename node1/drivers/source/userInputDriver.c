#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "../include/ADCDriver.h"
#include "../include/userInputDriver.h"
#include "../include/CANDriver.h"

JoystickOffset userInputInit(){
  JoystickOffset offset = calculateOffsetJoystick();
  return offset;
}

JoystickCoords calculateCalibratedJoystickCoords(JoystickOffset offset) {
  uint8_t rawX = readChannel(2);
  uint8_t rawY = readChannel(1);
  
  int16_t xValue = ((int16_t)rawX) - offset.x;
  int16_t yValue = ((int16_t)rawY) - offset.y;

  //saturation in offset, maintains strict interval [0,255]
  xValue = (xValue > 255) ? 255 : xValue;
  xValue = (xValue < 0) ? 0 : xValue;
  xValue = (yValue > 255) ? 255 : yValue;
  xValue = (yValue < 0) ? 0 : yValue;

  printf("JS_pre_map_x: %d \n\r",xValue);
  printf("JS_pre_map_y: %d \n\r",yValue);

  //Completely right should be 255, completely left should be 0
  JoystickCoords sampledValues;
  sampledValues.x = calculateJoystickMapping(xValue);
  sampledValues.y = calculateJoystickMapping(yValue);

  printf("JS_post_map_x: %d \n\r",sampledValues.x);
  printf("JS_post_map_y: %d \n\r",sampledValues.y);

  return sampledValues;
}

int8_t calculateJoystickMapping(int16_t rawValue) {
  if (rawValue >= 128) {
    return (int8_t)((rawValue-128 )/(128/100.0));
  }
  else {
    return (int8_t)((rawValue-128 )/(128/100.0));
  }
}

JoystickOffset calculateOffsetJoystick() {
  //optimization option: send address once, sample multiple times.
  //performance option: more samples, median of samples..

  //Sample a couple of times, do average, set as offset value
  int8_t totX = 0;
  int8_t totY = 0;
  for(int i = 0; i < 3; i++) {
    totX += (int16_t)readChannel(2) - 128;
    totY += (int16_t)readChannel(1) - 128;
  }
  JoystickOffset finalOffset;
  finalOffset.x = totX/3;
  finalOffset.y = totY/3;

  return finalOffset;
}

JoystickDir calculateJoystickDirection(JoystickCoords coords){
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


SliderPosition calculateSliderPosition(){

  SliderPosition position;
  uint8_t rawLeft = readChannel(3);
  uint8_t rawRight = readChannel(4);

  position.left = rawLeft;
  position.right = rawRight;

  return position;
}

uint8_t joystickButton(){
  return !(PINB & 0x01);
}

JoystickDir getCurrentJoystickDir(){
  JoystickOffset joystickOffset;
  joystickOffset = calculateOffsetJoystick();
  JoystickCoords joystickCoords;
  joystickCoords = calculateCalibratedJoystickCoords(joystickOffset);
  JoystickDir joystickDir;
  joystickDir = calculateJoystickDirection(joystickCoords);
  return joystickDir;
}


//MAPPING
// id for joystick = 1
// data[0] = x
// data[1] = y
// data[2] = button (LSB = button), 7 unused bits here


void send_joystick_position(JoystickOffset offset){
  //sends joystick position from node 1 to node 2
  struct CAN_msg msg;
  struct JoystickCoords coords;
  coords = calculateCalibratedJoystickCoords(offset);
  msg.id = 1;
  uint8_t array[8] = {coords.x,coords.y,joystickButton(),0,0,0,0,0};
  printf("ID: %d \n\r", msg.id);
  for (int j = 0; j < 8; j++){
    msg.data[j] = array[j];
    if (j < 3){
      printf("Data: %d , index : %d \n\r", msg.data[j], j);
    }
  }
  msg.length = 3;
  printf("Length: %d \n\r", msg.length);
  send_CAN_msg(&msg);

}
