#define F_CPU 16000000

#include <stdio.h>
#include <float.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "../include/TWI_Master.h"
#include "../include/motorDriver.h"
#include "../include/servoDriver.h"

void motor_init() {

  TWI_Master_Initialise();
  DDRK = 0x00;
  //TWCR |= (1 << TWIE);
  DDRH |= (1 << DDH1)|(1 << DDH3)|(1 << DDH4)|(1 << DDH5)|(1 << DDH6);
  PORTH |= (1<<PH4); // Enables motor
  PORTH &= ~(1<<PH3); // Output enable of encoder !OE
  PORTH &= ~(1<<PH6);
  _delay_ms(10);
  PORTH |= (1<<PH6);
  //PINH4 = 0xFF; // EN
  //PINH1 = 0xFF;// DIR
}

void motor_set_power(int16_t power) {

  unsigned char msgSize = 3;
  unsigned char msg[msgSize];
  unsigned char slave_address = 0b01011110;

  msg[0] = slave_address;
  msg[1] = 0;
  msg[2] = motor_set_direction_and_return_abs(power);

  TWI_Start_Transceiver_With_Data(msg, msgSize);
}


unsigned char motor_set_direction_and_return_abs(int16_t signed_power) {
  uint16_t unsigned_power;

  if (signed_power < 0){
    PORTH &= ~(1<<PH1); //sets dir to down
    unsigned_power = (uint16_t)(-1*(signed_power));
  }
  else {
    PORTH |= (1<<PH1); //sets dir to up
    unsigned_power = (uint16_t)(signed_power);
  }

  if(unsigned_power > 255){
    printf("|power| > 255!!! p: %d\n\r",signed_power);
    unsigned_power = 255;
  }

  return (unsigned char)unsigned_power;
}
// slave address for DAC:
// 0101 + AD0 + AD1 + AD2 + 0 // LSB 0 when writing to MAX520
// coords.y skal styre motorhastighet


uint16_t read_motor_encoder() {
  cli();
  PORTH &= ~(1<<PH5); // Output enable of encoder !OE
  PORTH &= ~(1<<PH3); // SEL high/low set low
  _delay_us(20);
  uint16_t encoder_counter = (PINK << 8); // read MSB
  PORTH |= (1<<PH3); // SEL high/low set high
  _delay_us(20);
  encoder_counter |= PINK; // read LSB

  //insert encoder reset toggle here, if wanting speed measures

  PORTH |= (1<<PH5); // Output disable of encoder !OE
  sei();

  return encoder_counter;
  // 2's complement from for negative numbers
}
