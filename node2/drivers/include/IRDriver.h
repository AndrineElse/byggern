struct IR_status {
  uint16_t IR_samples[5];
  uint8_t sample_counter;
  uint8_t current_sample_index;
};

void IR_init();
uint8_t IR_poll_failure();
void IR_add_sample();
uint16_t IR_get_mean();
void IR_reset_samples();