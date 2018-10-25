#include <avr/io.h>
#include "../include/servoDriver.h"
#include <avr/interrupt.h>
#include <stdint.h>
#define F_CPU 16000000
// calculate correct duty cycle/on time which will be provided to your time/counter driver
// implement safety features which will not let pwm go out of valid range for the servo


//
void servo_set_duty_cycle(uint8_t u){
  uint8_t d_0 = 56; //(0.9/20))*1249
  float u_scalar = 0.294; //(((u*1.0)/255)*(1.2/20))*1249;

  uint16_t duty_cycle = u*u_scalar + d_0;
  cli();
  OCR1A = duty_cycle;
  sei();
}
