/*struct JoystickCoords {
  int8_t x;
  int8_t y;
} typedef JoystickCoords;*/

void servo_set_duty_cycle(uint8_t u);
//JoystickCoords get_new_joystick_values();
void update_servo_position(JoystickCoords coords);
