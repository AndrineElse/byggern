struct PID_data {
  int32_t error_sum;
  float Kp;
  int8_t Ki;
  int16_t position;
  float sample_time;
  uint8_t encoder_factor;
  int16_t current_power;

  int16_t encoder_value;
};

void pos_controller_init(float p_factor, int8_t i_factor, float sample_time, uint8_t encoder_factor);
void pos_controller_calculate_power(int8_t reference_value, int16_t measured_value);
int16_t pos_controller_get_power();
