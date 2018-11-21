struct JoystickCoords {
  int8_t x;
  int8_t y;
} typedef JoystickCoords;

struct SliderPosition{
  uint8_t left;
  uint8_t right;
} typedef SliderPosition;

enum JoystickDir{NEUTRAL, LEFT, RIGHT, UP, DOWN} typedef JoystickDir;

void user_input_init();
SliderPosition get_slider_positions();
uint8_t get_slider_buttons();

void joystick_set_max_min_values();
JoystickCoords get_joystick_coords(uint8_t rawX, uint8_t rawY);
JoystickDir calculate_joystick_dir(JoystickCoords coords);
uint8_t joystick_get_button();
void user_input_send_CAN(uint8_t servoSignal, uint8_t controllerSignal);
uint8_t get_slider_position_right();
int8_t get_joystick_coords_x(uint8_t rawX);
int8_t get_joystick_coords_y(uint8_t rawY);
