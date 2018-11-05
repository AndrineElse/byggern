#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

void timer_init(){
  // we want:
  // * CTC mode,
  // * 10hz,
  // * an interrupt that increments a number for each match compare

  // using prescaler of 256
  // TOP has to be 0xC34 for the frequency to be 10Hz
  cli();
  // Set WGM to 0100 (CTC mode with OCRA containing top)
  // WGM 2:1 = 0b01,
  // CS3 2:0 = 0b100 (prescaler of 256)
  TCCR3B |= 0x0C;
  //WGM 1:0 = 0b00, all compare channels disabled
  TCCR3A |= 0x00;

  // Set OCR3A to contain 0xC34
  OCR3A = 0xC34;

  // Set OCIE3A to high, which enables the interrupt call when
  // a compare matches on OCR3A. This interrupt activates by setting
  // the corresponding flag OCF3A in TIFR3.
  // This flag clears automatically when the interrupt handler is called.
  TIMSK3 |= 0x02;
  sei();
}
//overwrite interrupt handler for OCF3A here
ISR(TIM3_COMPA_vect) {
  printf("yolo\n\r");
}

uint16_t get_time(){
  //donothing
  return 0;
}
