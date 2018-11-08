#include <util/delay.h>
#include <stdint.h>
#include "../../drivers/include/PWM_driver.h"
#include "../../drivers/include/servo_driver.h"
#include "../../drivers/include/CAN_driver.h"

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
