#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "ADCDriver.h"

//NB, this is dependent on SRAM_init already being run.
// Reads 8 bit discretized value, from channel 1-4
uint8_t readChannel(int channel) {
  volatile char* adc_ch1 = (char*) 0x1400;

  //Mapping of channels:
  // 4 -> channel 1 (Joystick, vertical)
  // 5 -> channel 2 (Joystick, horizontal)
  // 6 -> channel 3 (Slider, x)
  // 7 -> channel 4 (Slider, y)
  int mappedChannel = 3 + channel;

  // Telling ADC which channel to sample
  adc_ch1[0] = mappedChannel;

  // Waiting for ADC to sample value
  _delay_ms(60); //ADC raises an interrupt when value is calculated, this could be replaced with proper interrupt handling.

  //Pulling value
  uint8_t retreived_value = adc_ch1[0];
  return retreived_value;
}

JoystickCoords getCalibratedJoystickCoords(JoystickOffset offset) {
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

Dir 
