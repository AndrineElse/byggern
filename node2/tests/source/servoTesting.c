//system clock frequency, used by util/delay, 16MHz for node 2, 5MHz for node 1
#define F_CPU 16000000

#include <util/delay.h>
#include <stdint.h>
#include "../../drivers/include/PWMDriver.h"
#include "../../drivers/include/servoDriver.h"
#include "../../drivers/include/CANDriver2.h"
#include "../../containers/include/userInputContainer.h"

void servo_test(){
  /*
  servo_set_duty_cycle(0);
  _delay_ms(200000);
  servo_set_duty_cycle(255);
  _delay_ms(200000);
*/
  for (int i = 0; i < 256; i++){
    servo_set_duty_cycle(i);
    _delay_ms(1000);
  }
}

void servo_joystick_test(){
  servo_update_position(input_container_get_ptr()->joystick.x);
}
