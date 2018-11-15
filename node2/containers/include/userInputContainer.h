struct JoystickCoords {
  int8_t x;
  int8_t y;
} typedef JoystickCoords;

struct Input_status {
  struct JoystickCoords joystick;
  uint8_t joystickButton;
  uint8_t left_slider; // not in use
  uint8_t right_slider; // not in use
  uint8_t right_button; // not in use
  uint8_t left_button; // not in use
};

void input_container_init();
volatile struct Input_status* input_container_get_ptr();
void input_container_update(struct CAN_msg new_input_message);
void print_input_container();
