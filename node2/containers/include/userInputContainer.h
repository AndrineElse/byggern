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

volatile struct Input_status* get_input_ptr();
void input_update(struct CAN_msg new_input_message);