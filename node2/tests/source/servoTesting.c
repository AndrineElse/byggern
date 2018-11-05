#include <util/delay.h>
#include <stdint.h>
#include "../../drivers/include/PWMDriver.h"
#include "../../drivers/include/servoDriver.h"
#include "../../drivers/include/CANDriver2.h"

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
  update_servo_position(get_new_joystick_values());
}
