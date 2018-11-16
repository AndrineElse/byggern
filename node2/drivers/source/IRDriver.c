#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "../include/IRDriver.h"
#include "../include/ADCDriver2.h"

volatile struct IR_status IR_sample_container;

void IR_init() {
  IR_reset_samples();
}

void IR_reset_samples(uint8_t sample_amount) {
  IR_sample_container.current_sample_index = 0;
  IR_sample_container.sample_counter = 5;
  for (size_t i = 0; i < IR_sample_container.sample_counter; i++) {
    IR_sample_container.IR_samples[i] = 300;
  }
}


uint16_t IR_get_mean_value(){
  return IR_sample_container.current_mean;
}

uint8_t IR_check_failure(){
  return (IR_get_mean_value() < 100);
}

void IR_get_new_sample() {
  IR_sample_container.IR_samples[index] = adc_read();
  IR_sample_container.current_sample_index = (IR_sample_container.current_sample_index + 1)%IR_sample_container.sample_counter;
  uint16_t sum = 0;
  for (int i = 0; i < IR_sample_container.sample_counter; i++){
    sum += IR_sample_container.IR_samples[i];
  }
  IR_sample_container.current_mean = sum/(IR_sample_container.sample_counter);
}