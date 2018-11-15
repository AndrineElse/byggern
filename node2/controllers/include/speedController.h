struct PID_data {
  int32_t error_sum;
  int8_t Kp;
  int8_t Ki;
  int16_t position;
  float sample_time;
  uint8_t encoder_factor;
  int16_t current_power;
};

void speed_controller_init(int8_t p_factor, int8_t i_factor, float sample_time);
void speed_controller_calculate_power(int8_t reference_value, int16_t measured_value);
int16_t speed_controller_get_power();
