#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "../include/timerDriver.h"
#include "../../controllers/include/posController.h"
#include "../../containers/include/userInputContainer.h"
#include "../include/motorDriver.h"

volatile uint16_t tenths_of_second_counter;

void timer_hundred_ms_init(){
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
  tenths_of_second_counter = 0;
}

ISR(TIMER3_COMPA_vect) {
  tenths_of_second_counter++;
}

uint16_t time_get_counter(){
  return tenths_of_second_counter;
}

void timer_twenty_ms_init(){
  cli();

  TCCR0A = 0x02; // sets CTC mode, and no pin output

  TCCR0B = 0x05; //prescaler 1024, use for 50hz
  //TCCR0B = 0x04; //prescaler 256, use for 100hz

  //16MHz/(2*50Hz*1024) = 156 = (ocr+1)
  // => ocr = 155 = 0x9B

  OCR0A = 0x9B; //use for 50hz
  //OCR0A = 0x4D; //use for 100hz


  // Set OCIE0A to high, which enables the interrupt call when
  // a compare matches on OCR0A. This interrupt activates by setting
  // the corresponding flag OCF0A in TIFR0.
  // This flag clears automatically when the interrupt handler is called.
  TIMSK0 |= 0x02;

  sei();
}

ISR(TIMER0_COMPA_vect) {
  uint8_t pos_reference = input_container_get_ptr()->joystick.y + 100;
  //printf("Pos ref: %d\n\r", pos_reference);
  int16_t pos_measured = -1*read_motor_encoder();
  pos_controller_calculate_power(pos_reference, pos_measured);
}
