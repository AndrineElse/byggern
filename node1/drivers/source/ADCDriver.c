//System frequency used by util/delay, 16MHz for node 2, 5MHz for node 1
#define F_CPU 5000000

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "../include/ADCDriver.h"

//NB, this is dependent on SRAM_init already being run.
// Reads 8 bit discretized value, from channel 1-4
uint8_t readChannel(int channel) {
  volatile char* adc_ch1 = (char*) 0x1400;

  //Mapping of channels:
  // 4 -> channel 1 (Joystick, vertical)
  // 5 -> channel 2 (Joystick, horizontal)
  // 6 -> channel 3 (Slider, x, left)
  // 7 -> channel 4 (Slider, y, right)
  int mappedChannel = 3 + channel;
  // Telling ADC which channel to sample
  adc_ch1[0] = mappedChannel;

  // Waiting for ADC to sample value
  _delay_ms(60); //ADC raises an interrupt when value is calculated, this could be replaced with proper interrupt handling.

  //Pulling value
  uint8_t retreived_value = adc_ch1[0];
  return retreived_value;
}
