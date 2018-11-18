//System frequency used by util/delay, 16MHz for node 2, 5MHz for node 1
#define F_CPU 5000000

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "../include/ADCDriver.h"
#include "../include/userInputDriver.h"
#include "../include/CANDriver.h"
#include "../include/OLEDDriver.h"
#include "../include/timerDriver.h"

uint8_t maxX;
uint8_t maxY;
uint8_t minX;
uint8_t minY;
uint8_t centerX;
uint8_t centerY;
float y_above_scaler;
float y_below_scaler;
float x_above_scaler;
float x_below_scaler;

JoystickOffset userInputInit(){
  PORTB |= 1<<PB0; // set pinB0 as pull-up resistor input
  JoystickOffset offset = calculateOffsetJoystick();
  maxX = 0;
  maxY = 0;
  minX = 0;
  minY = 0;
  centerX = 0;
  centerY = 0;

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
  yValue = (yValue > 255) ? 255 : yValue;
  yValue = (yValue < 0) ? 0 : yValue;

  //Completely right should be 255, completely left should be 0
  JoystickCoords sampledValues;
  sampledValues.x = calculateJoystickMapping(xValue);
  sampledValues.y = calculateJoystickMapping(yValue);

  return sampledValues;
}

int8_t calculateJoystickMapping(int16_t rawValue) {
  return (int8_t)((rawValue-128 )/(128/100.0));
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

//returns value of slider buttons
// right button = LSB
// left button second least sign.b.
// buttons are connected to PD4 and PD5
uint8_t getSliderButtons(){
  uint8_t left_button_value = ( PIND & 0x10) >> 3; // shifts to one above lsb
  uint8_t right_button_value = ( PIND & 0x20) >> 5; //shifts to lsb
  return left_button_value + (right_button_value);
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


void send_joystick_position(){
  //sends joystick position from node 1 to node 2
  struct CAN_msg msg;
  struct JoystickCoords coords;
  coords = calculate_joystick_from_calibration(readChannel(2),readChannel(1));
  msg.id = 1;
  uint8_t array[8] = {coords.x,coords.y,joystickButton(),0,0,0,0,0};

  for (int j = 0; j < 8; j++){
    msg.data[j] = array[j];

  }
  msg.length = 3;
  send_CAN_msg(&msg);
  //printf("CoordsX: %d, CoordsY: %d, Button: %d\n\r", coords.x, coords.y, joystickButton());
}


void joystick_set_max_min_values(){
  char* options[5]= {"Set max right (x)", "Set min left (x)","Set max up (y)", "Set min down(y)", "Set center" };
  uint8_t i = 0;
  uint8_t flag = 0;

  while(i < 5){
    OLED_buffer_print_line(options[i], 1, 0);
    OLED_buffer_update_screen();
    if(flag == 0){
      uint8_t rawX = readChannel(2);
      uint8_t rawY = readChannel(1);
      printf("i: %d\n\r",i);
      switch (i) {

        case 0:
          maxX = rawX;
          break;
        case 1:
          minX = rawX;
          break;
        case 2:
          maxY = rawY;
          break;
        case 3:
          minY = rawY;
          break;
        case 4:
          centerX = rawX;
          centerY = rawY;
          break;
      }
      if(getSliderButtons() == 1){ //right slider button
        _delay_ms(500);
        i++;
      }
    }
  }
  //calculate slider scalers here.
  //scaler s should be a number such that
  //for pos_delta_x in [0,maxX-centerX]
  // => pos_delta_x*s in [0,100]
  x_above_scaler = ((float)100)/(maxX-centerX);
  x_below_scaler = ((float)100)/(centerX-minX);
  y_above_scaler = ((float)100)/(maxY-centerY);
  y_below_scaler = ((float)100)/(centerY-minY);
}

//after running calibration routine,
// this function will return joystick measures that are always [-100,100]
// returns (0,0) otherwise
JoystickCoords calculate_joystick_from_calibration(uint8_t rawX, uint8_t rawY) {
  JoystickCoords finalValues;
  //return naive calibration if calibration routine has not run
  if(!maxX){
    printf("JS not calibrated!");
    finalValues.x = (rawX - 128)/1.28;
    finalValues.y = (rawY - 128)/1.28;;
    return finalValues;
  }

  //shift values to center about zero
  int16_t centeredX = rawX - centerX;
  int16_t centeredY = rawY - centerY;

  //scale values to [-100,100]
  finalValues.x = (centeredX >= 0 ? centeredX*x_above_scaler : centeredX*x_below_scaler);
  finalValues.y = (centeredY >= 0 ? centeredY*y_above_scaler : centeredY*y_below_scaler);

  //in case measurements have drifted off from inital max/min values
  //probably wont need this, might be better to just run calibration scheme again
  finalValues.x = (finalValues.x >= 100 ? 100 : finalValues.x);
  finalValues.x = (finalValues.x < -100 ? -100 : finalValues.x);
  finalValues.y = (finalValues.y >= 100 ? 100 : finalValues.y);
  finalValues.y = (finalValues.y < -100 ? -100 : finalValues.y);

  printf("x:%d\n\r",finalValues.x);
  printf("y:%d\n\r",finalValues.y);
  return finalValues;
}
