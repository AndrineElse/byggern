#include "drivers/include/ADCDriver.h";
#include "drivers/include/userInputDriver.h"
#include <math.h>

JoystickDir calculateJoystickDirection_new(JoystickCoords joystickCoords) {
	double radius = sqrt(joystickCoords.x*joystickCoords.x + joystickCoords.y*joystickCoords.y);
	double dirInRadians = atan2(joystickCoords.x,joystickCoords.y); //goes from -pi to pi.
	dirInRadians = dirInRadians + (dirInRadians < 0 ? 2 * M_PI : 0); //goes from 0 to 2pi.
	if(radius > 30.0){
		if(dirInRadians > M_PI * 2 - M_PI_4) {
			return RIGHT;
		}
		if(dirInRadians > M_PI + M_PI_4){
			return DOWN;
		}
		if(dirInRadians > M_PI - M_PI_4){
			return LEFT;
		}
		if(dirInRadians > M_PI_4){
			return UP;
		}
		return RIGHT;
	}
	return NEUTRAL;
}