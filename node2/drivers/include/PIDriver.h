struct PID_data {
  int32_t error_sum;
  int8_t Kp;
  int8_t Ki;
  int16_t position;
};
void pid_init(int8_t p_factor, int8_t i_factor, struct PID_data *pid);
int16_t pid_controller(struct PID_data *pid);
