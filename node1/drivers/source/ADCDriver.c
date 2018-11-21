//System frequency used by util/delay, 16MHz for node 2, 5MHz for node 1
#define F_CPU 5000000

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "../include/ADCDriver.h"
#include "../include/userInputDriver.h"
#include "../../game/include/gameMenu.h"

//this flag states which channel is being converted,
//and must be zero (no conversion running) for an ad hoc conversion to be permitted
//this flag also serves as a state indicator for the conversion cycle,
uint8_t current_conversion_channel;
uint8_t current_joystick_x_sample;
uint8_t current_controller_signal_sample;

//TODO, make this a precompiled constant
volatile char* adc_ch1;

//enable low value interrupt on PD2 (int0) for completed ADC conversions
void ADC_init() {
  adc_ch1 = (char*)0x1400;

  //set int0 (PD2) to trigger on low value. (finished conversion)

  //set pd2 as input
  //donothing, should be input by default

  //enables int0
  GICR |= 0x40;

  //set int0 to trigger on low value
  //donothing, trigger on low value by default

  //initializing member variables
  current_conversion_channel = 0;
  current_joystick_x_sample = 0;
  current_controller_signal_sample = 0;
}

//Mapping of channels:
  // 4 -> channel 1 (Joystick, vertical)
  // 5 -> channel 2 (Joystick, horizontal)
  // 6 -> channel 3 (Slider, x, left)
  // 7 -> channel 4 (Slider, y, right)



//NB, this is dependent on SRAM_init already being run.
// Reads 8 bit discretized value, from channel 1-4
void ADC_start_conversion(uint8_t channel) {
  uint8_t mappedChannel = 3 + channel;
  // Telling ADC which channel to sample
  adc_ch1[0] = mappedChannel;
}



uint8_t ADC_read_current_channel() {
  if(current_conversion_channel & 0x8 || !current_conversion_channel){
    return 0;
  }
  return adc_ch1[0];
}



//NB, this is dependent on SRAM_init already being run.
// Reads 8 bit discretized value, from channel 1-4
uint8_t ADC_ad_hoc_read(uint8_t channel) {
  //stalls the program if there is a conversion cycle running
  while(current_conversion_channel);

  //sets a flag on the 4th bit (0x8)
  //to let the ISR the conversion can be disregarded
  cli();
  current_conversion_channel = 8 + channel;

  uint8_t mappedChannel = 3 + channel;

  // Telling ADC which channel to sample
  adc_ch1[0] = mappedChannel;

  // Waiting for ADC to sample value
  //TODO, try reducing this delay
  _delay_ms(10);

  //Pulling value
  uint8_t retrieved_value = adc_ch1[0];
  current_conversion_channel = 0;
  sei();

  return retrieved_value;

}



//joystick x and right slider are considered essential samples,
//these conversions are triggered by a timer,
//and a can message is sent as soon as
//both are finished
void ADC_start_conversion_cycle() {

  //dont bother calculating essential samples if not playing game
  if(!get_play_game()){
    user_input_send_can(0, 0);
    return;
  }

  //stall the program until adc is available
  //should not be necessary
  while(current_conversion_channel) {
    //there is no conceivable way to get into this loop...
    printf("waiting for reg convert\n\r");
  }

  current_conversion_channel = 2;
  ADC_start_conversion(current_conversion_channel);
}

//ad hoc conversions stop interrupt handling,
//so this function might want to run after an
// ad hoc conversion has finished
ISR(INT0_vect){
  //disregards the conversion interrupt if the conversion was ad hoc
  if(current_conversion_channel & 0x8){
    return;
  }

  switch(current_conversion_channel) {
    case 2:
      //first conversion finished, start second one
      current_joystick_x_sample = ADC_read_current_channel();

      //set this dependent on controller choice
      current_conversion_channel = (get_game_select_controller() ? 4 : 1);

      //start second conversion based on choice
      ADC_start_conversion(current_conversion_channel);
      break;
    case 4:
      //second conversion finished, send can message
      current_controller_signal_sample = ADC_read_current_channel();
      current_conversion_channel = 0;
      user_input_send_CAN(current_joystick_x_sample, current_controller_signal_sample);
      break;
    case 1:
      //second conversion finished, send can message
      current_controller_signal_sample = ADC_read_current_channel();
      current_conversion_channel = 0;
      user_input_send_CAN(current_joystick_x_sample, current_controller_signal_sample);
      break;
    default:
      break;
  }
}
