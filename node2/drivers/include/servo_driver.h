struct Joystick_coords {
  int8_t x;
  int8_t y;
} typedef Joystick_coords;

void servo_set_duty_cycle(uint8_t u);
Joystick_coords get_new_joystick_values();
void update_servo_position(Joystick_coords coords);
