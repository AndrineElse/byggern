struct IR_status {
  uint16_t IR_samples[5];
  uint8_t sample_counter;
  uint8_t current_sample_index;
};

void IR_container_init();
volatile struct IR_status* get_IR_container_ptr();