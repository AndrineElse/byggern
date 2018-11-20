//System frequency used by util/delay, 16MHz for node 2, 5MHz for node 1
#define F_CPU 5000000

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "../include/ADCDriver.h"
#include "../include/userInputDriver.h"
#include "../include/CANDriver.h"
#include "../include/OLEDDriver.h"
#include "../../game/include/gameMenu.h"
#include "../include/timerDriver.h"

uint8_t centerX;
uint8_t centerY;
float y_above_scaler;
float y_below_scaler;
float x_above_scaler;
float x_below_scaler;

void user_input_init(){
  PORTB |= 1<<PB0; // set pinB0 as pull-up resistor input
  centerX = 0;
  centerY = 0;
  y_above_scaler = 0;
  y_below_scaler = 0;
  x_above_scaler = 0;
  x_below_scaler = 0;
}

JoystickDir calculate_joystick_dir(JoystickCoords coords){
  if ((coords.x == coords.y) || (abs(abs(coords.x)-abs(coords.y)) < 20)){
    return NEUTRAL;
  }
  else if( coords.x > 83){
    return RIGHT;
  }
  else if (coords.x < -83){
    return LEFT;
  }
  else if(coords.y > 83){
    return UP;
  }
  else if(coords.y < -83){
    return DOWN;
  }
  return NEUTRAL;
}


SliderPosition get_slider_positions(){

  SliderPosition position;
  uint8_t rawLeft = readChannel(3);
  uint8_t rawRight = readChannel(4);

  position.left = rawLeft;
  position.right = rawRight;

  return position;
}

uint8_t get_slider_position_right(){
  return readChannel(4);
}

//returns value of slider buttons
// right button = LSB
// left button second least sign.b.
// buttons are connected to PD4 and PD5
uint8_t get_slider_buttons(){
  uint8_t left_button_value = ( PIND & 0x10) >> 3; // shifts to one above lsb
  uint8_t right_button_value = ( PIND & 0x20) >> 5; //shifts to lsb
  return left_button_value + (right_button_value);
}

//returns 1 if joystick button is pressed
//returns 0 otherwise
uint8_t joystick_get_button(){
  return !(PINB & 0x01);
}


void send_joystick_position(){
  struct CAN_msg msg;
  msg.id = 1;
  uint8_t array[8] = {get_joystick_coords_x(readChannel(2)),
                      get_slider_position_right(),
                      (joystick_get_button() + (get_play_game() << 1)+(get_restart_game() << 2)),
                      0,0,0,0,0};
  for (int j = 0; j < 8; j++){
    msg.data[j] = array[j];

  }
  msg.length = 3;
  cli();
  send_CAN_msg(&msg);
  sei();

}


void joystick_set_max_min_values(){
  char* options[5]= {"Set max right (x)", "Set min left (x)","Set max up (y)", "Set min down(y)", "Set center" };
  uint8_t i = 0;
  uint8_t flag = 0;
  uint8_t maxX;
  uint8_t maxY;
  uint8_t minX;
  uint8_t minY;

  while(i < 5){
    OLED_buffer_clear();
    OLED_buffer_print_line(options[i], 1, 0);
    OLED_buffer_update_screen();

    if(flag == 0){
      uint8_t rawX = readChannel(2);
      uint8_t rawY = readChannel(1);

      switch (i) {

        case 0:
          maxX = rawX;
          break;
        case 1:
          minX = rawX;
          break;
        case 2:
          maxY = rawY;
          break;
        case 3:
          minY = rawY;
          break;
        case 4:
          centerX = rawX;
          centerY = rawY;
          break;
      }
      if(get_slider_buttons() & 0x01){ //right slider button
        _delay_ms(500);
        i++;
      }
    }
  }
  OLED_buffer_clear();
  //scalers such that (measure-center)*scaler is in [-100,100]
  x_above_scaler = ((float)100)/(maxX-centerX);
  x_below_scaler = ((float)100)/(centerX-minX);
  y_above_scaler = ((float)100)/(maxY-centerY);
  y_below_scaler = ((float)100)/(centerY-minY);
}

// after running calibration routine,
// this function will return joystick measures that are always [-100,100]
// returns (0,0) otherwise
JoystickCoords get_joystick_coords(uint8_t rawX, uint8_t rawY) {
  JoystickCoords finalValues;
  //return naive calibration if calibration routine has not run
  if(!centerX){
    finalValues.x = (rawX - 128)/1.28;
    finalValues.y = (rawY - 128)/1.28;
    return finalValues;
  }

  //shift values to center about zero
  int16_t centeredX = rawX - centerX;
  int16_t centeredY = rawY - centerY;

  //scale values to [-100,100]
  finalValues.x = (centeredX >= 0 ? centeredX*x_above_scaler : centeredX*x_below_scaler);
  finalValues.y = (centeredY >= 0 ? centeredY*y_above_scaler : centeredY*y_below_scaler);

  //in case measurements have drifted off from inital max/min values
  //probably wont need this, might be better to just run calibration scheme again
  finalValues.x = (finalValues.x >= 100 ? 100 : finalValues.x);
  finalValues.x = (finalValues.x < -100 ? -100 : finalValues.x);
  finalValues.y = (finalValues.y >= 100 ? 100 : finalValues.y);
  finalValues.y = (finalValues.y < -100 ? -100 : finalValues.y);

  return finalValues;
}

uint8_t get_joystick_coords_x(uint8_t rawX) {
  uint8_t finalValue;
  //return naive calibration if calibration routine has not run
  if(!centerX){
    finalValue = (rawX - 128)/1.28;
    return finalValue;
  }

  //shift values to center about zero
  int16_t centeredX = rawX - centerX;


  //scale values to [-100,100]
  finalValue = (centeredX >= 0 ? centeredX*x_above_scaler : centeredX*x_below_scaler);

  //in case measurements have drifted off from inital max/min values
  //probably wont need this, might be better to just run calibration scheme again
  finalValue = (finalValue >= 100 ? 100 : finalValue);
  finalValue = (finalValue < -100 ? -100 : finalValue);
  return (uint8_t)finalValue;
}
