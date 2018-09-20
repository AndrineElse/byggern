struct JoystickCoords {
  int16_t x;
  int16_t y;
} typedef JoystickCoords;

struct SliderPosition{
  int16_t left;
  int16_t right;
  int8_t left_button;
  int8_t right_button;
} typedef SliderPosition;

struct JoystickOffset {
  int8_t x;
  int8_t y;
} typedef JoystickOffset;

enum JoystickDir{NEUTRAL, LEFT, RIGHT, UP, DOWN} typedef JoystickDir;

uint8_t readChannel(int channel);
JoystickCoords getCalibratedJoystickCoords();
int8_t calculateJoystickMapping(int16_t rawValue, int8_t offset);
JoystickOffset calculateOffsetJoystick();
JoystickDir calculateJoystickDirection();
SliderPosition calculateSliderPosition();
