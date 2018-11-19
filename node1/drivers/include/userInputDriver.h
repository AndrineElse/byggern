struct JoystickCoords {
  int8_t x;
  int8_t y;
} typedef JoystickCoords;

struct SliderPosition{
  uint8_t left;
  uint8_t right;
} typedef SliderPosition;

struct JoystickOffset {
  int8_t x;
  int8_t y;
} typedef JoystickOffset;

enum JoystickDir{NEUTRAL, LEFT, RIGHT, UP, DOWN} typedef JoystickDir;

void userInputInit();

SliderPosition get_slider_positions();
uint8_t get_slider_buttons();

void joystick_set_max_min_values();
JoystickCoords get_joystick_coords(uint8_t rawX, uint8_t rawY);
JoystickDir calculate_joystick_dir(JoystickCoords coords);
uint8_t joystick_get_button();
void user_input_send_can(uint8_t joystickX_reading, uint8_t slider_right_reading);
uint8_t get_slider_position_right();
uint8_t get_joystick_coords_x(uint8_t rawX);
