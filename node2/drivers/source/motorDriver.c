#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "../include/TWI_Master.h"
#include "../include/motorDriver.h"
#include "../include/servoDriver.h"
#include "../../containers/include/userInputContainer.h"


void motor_init(){
  TWI_Master_Initialise();
  //TWCR |= (1 << TWIE);
  DDRH |= (1 << DDH1)|(1 << DDH3)|(1 << DDH4)|(1 << DDH5)|(1 << DDH6);
  PORTH |= (1<<PH4); // Enables motor
  PORTH &= ~(1<<PH3); // Output enable of encoder !OE
  PORTH |= (1<<PH6);
  //PINH4 = 0xFF; // EN
  //PINH1 = 0xFF;// DIR
}

void set_motor_speed(){
  DDRD = 0x00;
  //JoystickCoords coords = get_new_joystick_values();
  JoystickCoords coords = get_input_ptr()->joystick
  unsigned char msgSize = 3;
  unsigned char msg[msgSize];
  unsigned char slave_address = 0b01011110;
  // slave_address |= (AD0 << 1) | (AD1 << 2) | (AD2 << 3) | (0101 << 4);

  msg[0] = slave_address;
  //msg[1] = (unsigned char)coords.y;
  msg[1] = 0;
  msg[2] = motor_vertical(coords.y);
  TWI_Start_Transceiver_With_Data(msg, msgSize);
  printf("Coords data: %d \n\r", msg[2]);
}


unsigned char motor_vertical(int8_t y){
  if (y < 0){
    PORTH &= ~(1<<PH1); //sets dir to down
    return (unsigned char)(-1*(y));
  }
  else if (y > 0){
    PORTH |= (1<<PH1); //sets dir to up
    return (unsigned char)(y);
  }
  return 0;
}
// slave address for DAC:
// 0101 + AD0 + AD1 + AD2 + 0 // LSB 0 when writing to MAX520
// coords.y skal styre motorhastighet

unsigned char motor_contoller(){

}

uint16_t read_motor_encoder(){
  cli();
  DDRK = 0x00;
  printf("PINK: %d\n\r", PINK);
  PORTH &= ~(1<<PH5); // Output enable of encoder !OE
  PORTH &= ~(1<<PH3); // SEL high/low set low
  _delay_ms(20);
  uint16_t encoder_counter = (PINK << 8); // read MSB
  printf("MSB: %d\n\r", encoder_counter);
  PORTH |= (1<<PH3); // SEL high/low set high
  _delay_ms(20);
  encoder_counter |= PINK; // read LSB
  printf("LSB: %d\n\r", encoder_counter);
  PORTH &= ~(1<<PH6); // toggle RST
  _delay_ms(20);
  PORTH |= (1<<PH6);
  PORTH |= (1<<PH5); // Output disable of encoder !OE
  sei();

  return encoder_counter;
  // 2's complement from for negative numbers
}
