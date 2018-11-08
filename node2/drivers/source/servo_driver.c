#include <avr/io.h>
#include "../include/servo_driver.h"
#include "../include/PWM_driver.h"
#include "../include/CAN_driver.h"
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

Joystick_coords get_new_joystick_values(){
  Joystick_coords coords;
  struct CAN_msg received_message = CAN_receive_msg();
  if (received_message.id == 1){ //Because joystick messages has id 1
    int8_t x = received_message.data[0];
    int8_t y = received_message.data[1];
    // uint8_t x_transform = (x*1.28)+128;
    coords.x = x;
    coords.y = y;
  }
  return coords;
}

void update_servo_position(Joystick_coords coords){
  uint8_t x_transform = (coords.x*1.28)+128;
  servo_set_duty_cycle(x_transform);
}
