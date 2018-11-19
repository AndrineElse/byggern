void motor_init();
void motor_set_power(int16_t power, uint16_t saturation);
unsigned char motor_set_direction_and_return_abs(int16_t y, uint16_t saturation);
uint16_t read_motor_encoder();
void motor_encoder_reset();
int16_t motor_get_max_encoder();
void saturation_set(uint16_t sat);
