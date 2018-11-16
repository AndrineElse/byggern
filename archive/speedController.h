struct PID_data {
  int32_t error_sum;
  float Kp;
  float Ki;
  int16_t position;
  float sample_time;
  uint8_t encoder_factor;
  int16_t current_power;
  int16_t last_encoder_value;
};

void speed_controller_init(float p_factor, uint8_t encoder_factor);
void speed_controller_calculate_power(int8_t reference_value, int16_t measured_value);
int16_t speed_controller_get_power();
