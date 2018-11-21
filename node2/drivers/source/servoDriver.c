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
  uint8_t d_0 = 43; //(0.9/20))*1249  // originally 56
  float u_scalar = 0.294; //(((u*1.0)/255)*(1.2/20))*1249;
  uint16_t duty_cycle = u*u_scalar + d_0;

  //implementing pulse width saturation
  duty_cycle = (duty_cycle < 56 ? 56 : duty_cycle);
  duty_cycle = (duty_cycle > 131 ? 131 : duty_cycle);


  cli();
  OCR1A = duty_cycle;

  //force pb5 low to avoid missing the compare match
  //arbitrary margin chosen after some testing
  if(TCNT1 + 2 > duty_cycle){
    PINB &= ~0x20;
  }
  sei();
}

void servo_update_position(int8_t setting){
  uint8_t uint8_transform = (setting*1.27)+128;
  servo_set_duty_cycle(uint8_transform);
}
