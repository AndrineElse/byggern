struct JoystickCoords {
  int16_t x;
  int16_t y;
} typedef JoystickCoords;

struct SliderPosition{
  int16_t left;
  int16_t right;
} typedef SliderPosition;

struct JoystickOffset {
  int8_t x;
  int8_t y;
} typedef JoystickOffset;

enum JoystickDir{NEUTRAL, RIGHT, UP, LEFT, DOWN} typedef JoystickDir;


JoystickCoords calculateCalibratedJoystickCoords();
int8_t calculateJoystickMapping(int16_t rawValue, int8_t offset);
JoystickOffset calculateOffsetJoystick();
JoystickDir calculateJoystickDirection();
SliderPosition calculateSliderPosition();
uint8_t joystickButton();