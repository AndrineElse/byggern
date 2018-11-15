#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "../include/servoDriver.h"
#include "../include/PWMDriver.h"

// sets the duty cycle on the pwm,
// with pulses according to u:
//    u=0 -> 0.9ms pulse -> max left angle
//    u=128 -> 1.5 ms pulse -> center angle
//    u=255 -> 2.1ms pulse -> max right angle
void servo_set_duty_cycle(uint8_t u){

  //maps the uint8 into a uint16 where the min/max
  //  values correspond to 0.9 ms to 2.1 ms pulses
  uint8_t d_0 = 56; //(0.9/20))*1249  // originally 43
  float u_scalar = 0.294; //(((u*1.0)/255)*(1.2/20))*1249;
  uint16_t duty_cycle = u*u_scalar + d_0;

  //TODO, add a saturation test here, to ensure only safe pulses

  cli(); //interrupt disable due to 16 bit write action
  OCR1A = duty_cycle;
  sei(); //interrupts safe again
}

//updates servo angle,
//setting should be a number in [-100, 100]
//  where 0 is the servo being straight.
void servo_update_position(int8_t setting){
  //transforms to an uint8,
  //  where max/min is 0,255
  //  and center is 128
  uint8_t uint8_transform = (setting*1.28)+128;
  servo_set_duty_cycle(uint8_transform);
}
