struct IR_status {
  uint8_t IR_samples[5];
  uint8_t sample_counter;
  uint8_t current_sample_index;
};
uint8_t get_IR_mean_value(struct IR_status* status);
uint8_t IR_poll_failure(uint8_t* last_IR_value, struct IR_status* IR_sample_container);
void IR_init(struct IR_status* IR_sample_container);
