struct PID_data {
  int32_t error_sum;
  int8_t Kp;
  int8_t Ki;
  float sample_time;
  int16_t current_power;
  uint16_t pi_container.encoder_max;
  uint8_t pi_container.encoder_scale;
};

void pos_controller_init(int8_t p_factor, int8_t i_factor, float sample_time, uint8_t encoder_max);
void pos_controller_calculate_power(int8_t reference_value, int16_t measured_value);
int16_t pos_controller_get_power();
