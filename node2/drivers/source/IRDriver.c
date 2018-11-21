#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "../include/IRDriver.h"
#include "../include/ADCDriver2.h"

volatile struct IR_status IR_sample_container;

void IR_init(uint8_t sample_amount) {
  IR_sample_container.sample_counter = sample_amount;
  IR_reset_samples();
}

void IR_reset_samples() {
  IR_sample_container.current_sample_index = 0;
  for (size_t i = 0; i < IR_sample_container.sample_counter; i++) {
    IR_sample_container.IR_samples[i] = 300;
  }
}


uint16_t IR_get_mean_value(){
  return IR_sample_container.current_mean;
}

uint8_t IR_check_obstruction(){
  return IR_get_mean_value() < 100; //&& IR_get_oldest_sample_delta() > 100);
}

void IR_get_new_sample() {
  IR_sample_container.IR_samples[IR_sample_container.current_sample_index] = adc_read();
  IR_sample_container.current_sample_index = (IR_sample_container.current_sample_index + 1)%IR_sample_container.sample_counter;
  uint16_t sum = 0;
  for (int i = 0; i < IR_sample_container.sample_counter; i++){
    sum += IR_sample_container.IR_samples[i];
  }
  IR_sample_container.current_mean = sum/(IR_sample_container.sample_counter);
}
