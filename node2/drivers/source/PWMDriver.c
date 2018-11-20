#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "../include/PWMDriver.h"
#include "../include/servoDriver.h"



void pwm_init(){
  DDRB |= (1<<DDB5);
  //WGM00 and WGM01 is set
  TCCR1B |= 0x18;
  //WGM02 is set and COM1A1
  TCCR1A |= 0x82;
  //WGM03 is set and COM1A0 to zero
  TCCR1A &= 0xBE;

  //Set the prescaler to 256
  TCCR1B |= 0x04;
  TCCR1B &= 0xFC;

  cli();
  ICR1 = 0x4E1;

  OCR1A = 0;
  
  sei();
  servo_set_duty_cycle(128);
}
