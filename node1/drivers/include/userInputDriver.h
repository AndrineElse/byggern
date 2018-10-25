struct JoystickCoords {
  int8_t x;
  int8_t y;
} typedef JoystickCoords;

struct SliderPosition{
  int16_t left;
  int16_t right;
} typedef SliderPosition;

struct JoystickOffset {
  int8_t x;
  int8_t y;
} typedef JoystickOffset;

enum JoystickDir{NEUTRAL, LEFT, RIGHT, UP, DOWN} typedef JoystickDir;

JoystickOffset userInputInit();
JoystickCoords calculateCalibratedJoystickCoords(JoystickOffset offset);
int8_t calculateJoystickMapping(int16_t rawValue);
JoystickOffset calculateOffsetJoystick();
JoystickDir calculateJoystickDirection();
SliderPosition calculateSliderPosition();
uint8_t joystickButton();
JoystickDir getCurrentJoystickDir();
void send_joystick_position(JoystickOffset offset);
