void motor_init(struct PID_data* pid);
void set_motor_speed(struct PID_data* pid);
unsigned char motor_vertical(int8_t y);
uint16_t read_motor_encoder();
