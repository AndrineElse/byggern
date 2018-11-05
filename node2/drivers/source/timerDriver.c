#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

void timer_init(){
  //WGM00 and WGM01 is set
  TCCR3B |= 0x00;
  //WGM02 is set and COM1A1
  TCCR3A |= 0x80;
  //WGM03 is set and COM1A0 to zero
  TCCR3A &= 0xBE;
  // Assume that WGM03 is 1?

  //Set the prescaler to 256
  TCCR3B |= 0x04;
  TCCR3B &= 0xFC;
}

uint16_t get_time(){
  cli();
  // ICR1 = 0x4E1; input capture register, used for defining counter TOP value
  uint16_t clock_count = TCNT3L;
  clock_count |= (TCNT3H << 7);
  sei();
  return (clock_count*50);
}
