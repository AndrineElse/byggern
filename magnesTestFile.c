#include "ADCDriver.h";
#include <math.h>
JoystickDir calculateJoystickDirection_new(JoystickCoords joystickCoords) {
	double radius = sqrt(joystickCoords.x*joystickCoords.x + joystickCoords.y*joystickCoords.y);
	double dirInRadians = atan2(joystickCoords.x,joystickCoords.y); //goes from -pi to pi.
	dirInRadians = dirInRadians + (dirInRadians < 0 ? 2*M_PI : 0);
	if(radius > 30.0){
		if(dirInRadians)
	}
	return NEUTRAL;
}