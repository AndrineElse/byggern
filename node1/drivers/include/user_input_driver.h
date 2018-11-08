struct Joystick_coords {
  int8_t x;
  int8_t y;
} typedef Joystick_coords;

struct Slider_position{
  int16_t left;
  int16_t right;
} typedef Slider_position;

struct Joystick_offset {
  int8_t x;
  int8_t y;
} typedef Joystick_offset;

enum Joystick_dir{NEUTRAL, LEFT, RIGHT, UP, DOWN} typedef Joystick_dir;

Joystick_offset user_input_init();
Joystick_coords get_calibrated_joystick_coords(Joystick_offset offset);
int8_t get_joystick_mapping(int16_t raw_value);
Joystick_offset get_offset_joystick();
Joystick_dir get_joystick_direction();
Slider_position get_slider_position()();
uint8_t get_joystick_button();
Joystick_dir get_current_joystickDir();
void send_joystick_position_CAN(Joystick_offset offset);
