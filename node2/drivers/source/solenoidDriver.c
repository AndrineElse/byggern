#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <float.h>
#include <avr/interrupt.h>
#include "../include/solenoidDriver.h"
#include "../include/CANDriver2.h"

// data[2] = button (LSB = button), 7 unused bits here

void solenoid_init(){
  DDRF |= (1<<DDF1);
  PINF &= ~(1<<PF1);
}

uint8_t button_pressed(){
  //struct CAN_msg msg = receive_msg();
  //uint8_t pressed = msg.data[2];
  uint8_t pressed = input_container_get_ptr()->joystickButton;
  printf("Button: %d\n", pressed);
  return pressed;
}

void solenoid_trigger(){
  if (button_pressed() == 1){
    PINF |= (1<<PF1);
    _delay_ms(500);
    PINF &= ~(1<<PF1);
    _delay_ms(50);
  }



  // solenoid at ADC1 = PF1
}
