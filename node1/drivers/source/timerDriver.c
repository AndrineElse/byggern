#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../include/timerDriver.h"
#include "../../game/include/gameMenu.h"
#include "../include/userInputDriver.h"

volatile uint16_t tenths_of_second_counter;

void timer_init() {
  // we want:
  // * CTC mode,
  // * 10hz,
  // * an interrupt that increments a number for each match compare

  // using prescaler of 256
  // TOP has to be 0x7A2 for the frequency to be 10Hz
  cli();
  // Set WGM to 0100 (CTC mode with OCRA containing top)
  // WGM 2:1 = 0b01,
  // CS3 2:0 = 0b100 (prescaler of 256)
  TCCR3B |= 0x0C;
  //WGM 1:0 = 0b00, all compare channels disabled
  TCCR3A |= 0x00;

  // Set OCR3A to contain 0x7A2
  OCR3A = 0x7A2;

  // Set OCIE3A to high, which enables the interrupt call when
  // a compare matches on OCR3A. This interrupt activates by setting
  // the corresponding flag OCF3A in TIFR3.
  // This flag clears automatically when the interrupt handler is called.
  ETIMSK |= 0x10;
  sei();
  tenths_of_second_counter = 0;
}

ISR(TIMER3_COMPA_vect) {
	//printf("time int!");
  /*
  if(!(tenths_of_second_counter%10)){
    game_send_data_CAN();
  }
  */
  tenths_of_second_counter++;
  /*
  if(!(tenths_of_second_counter%3)){
      send_joystick_position();
  }
  */
  ADC_start_conversion_cycle();

}

uint16_t timer_get_counter() {
	return tenths_of_second_counter;
}
