struct PID_data {
  int32_t error_sum;
  double Kp;
  double Ki;
  int16_t position;
};
void pid_init(double p_factor, double i_factor, struct PID_data *pid);
int16_t pid_controller(struct PID_data *pid);
