#include <stdint.h>
#include "../include/IRSampleContainer.h" 

volatile struct IR_status IR_status_container;

//initializes container
void IR_container_init() {
  IR_status_container.current_sample_index = 0;
  IR_status_container.sample_counter = 5;
  for (uint8_t i = 0; i < IR_status_container.sample_counter; i++) {
    IR_status_container.IR_samples[i] = 300;
  }
}

//returns pointer to whole sample container
volatile struct IR_status* get_IR_container_ptr() {
	return &IR_status_container;
}