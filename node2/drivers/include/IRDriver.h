struct IR_status {
  uint16_t IR_samples[5];
  uint8_t sample_counter;
  uint8_t current_sample_index;
  uint8_t current_mean;
};

uint16_t IR_get_mean_value();
uint8_t IR_check_failure();
void IR_init();
void IR_get_new_sample();
void IR_reset_samples();
