#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <float.h>
#include <avr/interrupt.h>
#include "../include/solenoidDriver.h"
#include "../include/CANDriver2.h"
#include "../include/timerDriver.h"
#include "../../containers/include/userInputContainer.h"

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
  if (button_pressed()){
    PINF |= (1<<PF1);
  }
  // solenoid at ADC1 = PF1
}

void solenoid_update_status(uint8_t* button_flag, uint16_t* timer){
  if(*button_flag == 0){
    if(button_pressed()){
      uint16_t start_time = *timer;
      *button_flag = 1;
      solenoid_trigger();
      while((*timer - start_time)<50){
        printf("Counter: %d\n\r", time_get_counter() );
      }
      PINF &= ~(1<<PF1);
      *timer = 0;
      *button_flag=0;
    }
  }

}
