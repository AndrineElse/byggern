#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include "../include/IRDriver.h"
#include "../include/ADCDriver2.h"

/*
MAPPING
IR_Value > 100 : No fail
IR_Value < 100 : fail

///

if last_IR_value - current_IR_value > 100

*/



void IR_init(struct IR_status* IR_sample_container){
  IR_sample_container->current_sample_index = 0;
  IR_sample_container->sample_counter = 5;
  for (size_t i = 0; i < IR_sample_container->sample_counter; i++) {
    IR_sample_container->IR_samples[i] = 255;
  }
}

uint8_t get_IR_mean_value(struct IR_status* status){
  uint8_t mean_current_IR_value = 0;
  uint8_t index = status->current_sample_index;
  status->IR_samples[index] = adc_read();
  printf("IR_sample: %d\n\r", status->IR_samples[index]);
  status->current_sample_index = (index + 1)%5;
  uint16_t sum = 0;
  for (int i = 0; i < status->sample_counter; i++){
    sum += status->IR_samples[i];
  }
  return (sum/status->sample_counter);
}

uint8_t IR_poll_failure(struct IR_status* IR_sample_container){
  uint8_t current_IR_value = get_IR_mean_value(IR_sample_container);
  printf("Current: %d\n\r", current_IR_value);
  if (current_IR_value < 100){
    //*last_IR_value = current_IR_value;
    return 1;
  }
  //*last_IR_value = current_IR_value;
  return 0;
}
