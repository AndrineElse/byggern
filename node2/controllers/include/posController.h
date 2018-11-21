struct PID_data {
  int32_t error_sum;
  int8_t Kp;
  int8_t Ki;
  float sample_time;
  int16_t current_power;
  uint16_t encoder_max;
  uint8_t encoder_scale;
  int16_t last_encoder_value;
  int16_t power_log[5];
  uint8_t power_log_index;
};

void pos_controller_init(int8_t p_factor, int8_t i_factor, float sample_time);
void pos_controller_update(int8_t p_factor, int8_t i_factor);
void pos_controller_calculate_power(uint8_t reference_value, int16_t measured_value);
int16_t pos_controller_get_power();
void pos_controller_reset();
void pos_controller_log_current_power();
int16_t pos_controller_get_avg_power();
