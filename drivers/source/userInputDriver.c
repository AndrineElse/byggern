#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "../include/ADCDriver.h"
#include "../include/userInputDriver.h"

JoystickCoords calculateCalibratedJoystickCoords(JoystickOffset offset) {
  uint8_t rawX = readChannel(2);
  uint8_t rawY = readChannel(1);

  int16_t xValue = (int16_t)(rawX) - offset.x;
  int16_t yValue = (int16_t)(rawY) - offset.y;

  //Completely right should be 255, completely left should be 0
  JoystickCoords sampledValues;
  sampledValues.x = calculateJoystickMapping(xValue, offset.x);
  sampledValues.y = calculateJoystickMapping(yValue, offset.y);
  return sampledValues;
}

int8_t calculateJoystickMapping(int16_t rawValue, int8_t offset) {
  if(rawValue>= 128){
    return (int8_t)((rawValue-128 )/((128-offset)/100.0));
  }
  else{
    return (int8_t)((rawValue-128 )/((128+offset)/100.0));
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
