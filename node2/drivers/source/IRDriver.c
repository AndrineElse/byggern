#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include "../include/IRDriver.h"
#include "../include/ADCDriver2.h"
#include "../../containers/include/IRSampleContainer.h"

// sets reasonable inital values
void IR_init(){
  IR_container_init();
}

// returns 1 if there is an obstruction. 
// returns 0 otherwise
uint8_t IR_poll_failure(){
  if (IR_get_mean() < 100){
    return 1;
  }
  return 0;
}


//adds a new sample to the container, increments index.
void IR_add_sample() {
  uint8_t index = get_IR_container_ptr()->current_sample_index;
  get_IR_container_ptr()->IR_samples[index] = adc_read();
  get_IR_container_ptr()->current_sample_index = (index + 1)%5;
}

//returns mean of last n samples, where n=sample_counter
uint16_t IR_get_mean() {
  uint16_t sum = 0;
  for (int i = 0; i < get_IR_container_ptr()->sample_counter; i++){
    sum += get_IR_container_ptr()->IR_samples[i];
  }
  return (sum/get_IR_container_ptr()->sample_counter);
}

//resets samples, to ensure a reasonable mean after a failure.
void IR_reset_samples() {
  for (uint8_t i = 0; i < get_IR_container_ptr()->sample_counter; i++) {
    get_IR_container_ptr()->IR_samples[i] = 300;
  }
}

