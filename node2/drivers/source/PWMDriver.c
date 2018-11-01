#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "../include/PWMDriver.h"
#include "../include/servoDriver.h"



void pwm_init(){
  // Set output
  DDRB |= (1<<DDB5);
  //WGM00 and WGM01 is set
  TCCR1B |= 0x18;
  //WGM02 is set and COM1A1
  TCCR1A |= 0x82;
  //WGM03 is set and COM1A0 to zero
  TCCR1A &= 0xBE;
  // Assume that WGM03 is 1?

  //Set the prescaler to 256
  TCCR1B |= 0x04;
  TCCR1B &= 0xFC;

  //Set frequency to 50Hz

  /*
  OCR1AL = 0x0;
  OCR1AH = 0x5;
  */
  //char sreg = SREG;
  //__disable_interrupt();
  cli();
  ICR1 = 0x4E1;


  OCR1A = 0;
  //OCR1A = 0x50;
  //OCR1B = 0x20;
  sei();
  //SREG = sreg;
  servo_set_duty_cycle(128);
}
