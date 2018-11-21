struct IR_status {
  uint16_t IR_samples[10];
  uint8_t sample_counter;
  uint8_t current_sample_index;
  uint16_t current_mean;
};

uint16_t IR_get_mean_value();
uint8_t IR_check_obstruction();
void IR_init(uint8_t sample_amount);
void IR_get_new_sample();
void IR_reset_samples();
uint16_t IR_get_oldest_sample_delta();
