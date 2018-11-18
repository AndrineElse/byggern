void motor_init();
void motor_set_power(int16_t power);
unsigned char motor_set_direction_and_return_abs(int16_t y);
uint16_t read_motor_encoder();
void motor_encoder_reset();
void motor_set_max_min_encoder(uint8_t dir);
void motor_calibrate_encoder();
