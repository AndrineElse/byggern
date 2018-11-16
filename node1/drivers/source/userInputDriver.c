//System frequency used by util/delay, 16MHz for node 2, 5MHz for node 1
#define F_CPU 5000000

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "../include/ADCDriver.h"
#include "../include/userInputDriver.h"
#include "../include/CANDriver.h"
#include "../include/OLEDDriver.h"
#include "../include/timerDriver.h"
//volatile ??
volatile JoystickCoords max_coords;
volatile JoystickCoords min_coords;

JoystickOffset userInputInit(){
  PORTB |= 1<<PB0; // set pinB0 as pull-up resistor input
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
  //uint8_t left_button_value = ( PIND & (1<<PD4)) >> PD4;
  //uint8_t right_button_value = ( PIND & (1<<PD5)) >> PD5;
  uint8_t left_button_value = ( PIND & 0x10) >> 3;
  uint8_t right_button_value = ( PIND & 0x20) >> 5;
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


void send_joystick_position(JoystickOffset offset, uint16_t *timer, uint8_t *flag){
  if (*flag == 0){
    *timer = timer_get_counter();
    *flag = 1;
  }
  else{
    if((timer_get_counter() - *timer) > 10){
      struct CAN_msg msg;
      struct JoystickCoords coords;
      coords = calculateCalibratedJoystickCoords(offset);
      msg.id = 1;
      uint8_t array[8] = {coords.x,coords.y,joystickButton(),0,0,0,0,0};

      for (int j = 0; j < 8; j++){
        msg.data[j] = array[j];

      }
      msg.length = 3;
      cli();
      send_CAN_msg(&msg);
      sei();
      *flag = 0;
      *timer = 0;
    }
  }
}
  //sends joystick position from node 1 to node 2

  //rintf("CoordsX: %d, CoordsY: %d, Button: %d\n\r", coords.x, coords.y, joystickButton());


void joystick_set_max_min_values(){
  char* options[4]= {"Set max right (x)", "Set min left (x)","Set max up (y)", "Set min down(y)" };
  /*
  [20];
  options = {"Set max right (x)", "Set min left (x)","Set max up (y)", "Set min down(y)" };*/
  for (uint8_t i = 0; i < 4; i++) {
    OLED_buffer_print_line(options[i], 1, 0);
    OLED_buffer_update_screen();
    while ((!getSliderButtons())) {

      uint8_t rawX = readChannel(2);
      uint8_t rawY = readChannel(1);
      switch (i) {
        case 1:
          max_coords.x = rawX;
          break;
        case 2:
          min_coords.x = rawX;
          break;
        case 3:
          min_coords.x = rawX;
          break;
        case 4:
          min_coords.x = rawX;
          break;
      }
      if(getSliderButtons() == 1){
        _delay_ms(1000);
      }
      //printf("Max x: %d, Max y: %d, min x: %d, min y: %d \n\r", max_coords.x, max_coords.y,min_coords.x,min_coords.y);
    }
    OLED_buffer_clear();
  }
  //printf("Max x: %d, Max y: %d, min x: %d, min y: %d", max_coords.x, max_coords.y,min_coords.x,min_coords.y);
}
/*
JoystickCoords joystick_calibration(JoystickCoords joystickMax){


}
*/
