#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <float.h>
#include <avr/interrupt.h>
#include "../include/solenoidDriver.h"
#include "../include/CANDriver2.h"

// data[2] = button (LSB = button), 7 unused bits here

uint8_t button_pressed(){
  struct CAN_msg msg = receive_msg();
  uint8_t pressed = msg.data[2];
  printf("Button: %d\n", pressed);
  return pressed;
}

void solenoid_trigger(){
  DDRF |= (1<<DDF1);
  if (button_pressed() == 1){
    PINF |= (1<<PF1);
    _delay_ms(10000);
    PINF &= ~(1<<PF1);
  }


  // solenoid at ADC1 = PF1
}
