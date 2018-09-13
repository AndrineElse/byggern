struct JoystickCoords {
  int16_t x;
  int16_t y;
} typedef JoystickCoords;

struct JoystickOffset {
  int8_t x;
  int8_t y;
} typedef JoystickOffset;

enum Dir{NEUTRAL, LEFT, RIGHT, UP, DOWN} typedef dir;

uint8_t readChannel(int channel);
JoystickCoords getCalibratedJoystickCoords();
int8_t calculateJoystickMapping(int16_t rawValue, int8_t offset);
JoystickOffset calculateOffsetJoystick();
