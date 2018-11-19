#include <avr/io.h>
#include <stdio.h>
#include <float.h>
#include <avr/interrupt.h>
#include "../include/solenoidDriver.h"
#include "../include/CANDriver2.h"
#include "../include/timerDriver.h"
#include "../../containers/include/userInputContainer.h"

// data[2] = button (LSB = button), 7 unused bits here

uint16_t solenoid_timer = 0;

void solenoid_init(){
  DDRF |= (1<<DDF1);
  PORTF &= ~(1<<PF1);
  solenoid_timer = 0;
}

void solenoid_update_status(){
  if (!timer && input_container_get_ptr()->joystickButton) {
    timer = time_get_counter();
    PORTF |= (1<<PF1);
  }
  else if ((time_get_counter() - timer) > 1) {
    PORTF &= ~(1<<PF1);
    timer = 0;
  }
}

void solenoid_hard_stop(){
  PORTF &= ~(1<<PF1);
  timer = 0;
}
