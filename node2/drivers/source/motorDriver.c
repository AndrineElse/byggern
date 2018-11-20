#define F_CPU 16000000

#include <stdio.h>
#include <float.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "../include/TWI_Master.h"
#include "../include/motorDriver.h"
#include "../include/servoDriver.h"
#include "../../controllers/include/posController.h"

int16_t min_encoder;
int16_t max_encoder;
uint16_t power_saturation;

void motor_init() {

  TWI_Master_Initialise();
  DDRK = 0x00;
  DDRH |= (1 << DDH1)|(1 << DDH3)|(1 << DDH4)|(1 << DDH5)|(1 << DDH6);
  PORTH |= (1<<PH4); // Enables motor
  PORTH &= ~(1<<PH3); // Output enable of encoder !OE
  PORTH &= ~(1<<PH6);
  _delay_ms(10);
  PORTH |= (1<<PH6);
  motor_set_power(0);
  power_saturation = 100;
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

  if(unsigned_power > power_saturation){
    unsigned_power = power_saturation;
  }

  return (unsigned char)unsigned_power;
}


uint16_t read_motor_encoder() {
  cli();
  PORTH &= ~(1<<PH5); // Output enable of encoder !OE
  PORTH &= ~(1<<PH3); // SEL high/low set low
  _delay_us(20);
  uint16_t encoder_counter = (PINK << 8);
  PORTH |= (1<<PH3); // SEL high/low set high
  _delay_us(20);
  encoder_counter |= PINK; 

  PORTH |= (1<<PH5); // Output disable of encoder !OE
  sei();

  return encoder_counter;
}


void motor_encoder_reset(){
    PORTH &= ~(1<<PH6);
    _delay_us(50);
    PORTH |= 1<<PH6;
    _delay_us(50);
}

int16_t motor_get_max_encoder(){
  motor_encoder_reset();
  motor_set_power(0);
  int16_t last_encoder_value = 100;
  int16_t current_encoder_value;
  uint8_t count = 0;
  //Drive the motor to the opposite side
  motor_set_power(80);
  while(count < 10){
    _delay_ms(20);
    current_encoder_value = read_motor_encoder();
    if (current_encoder_value == last_encoder_value){
      count ++;
    }
    last_encoder_value = current_encoder_value;
  }
  motor_set_power(0);
  motor_encoder_reset();
  motor_set_power(-80);

  count = 0;
  last_encoder_value = 100;
  current_encoder_value;
  while(1){
    _delay_ms(20);
    current_encoder_value = (1)*read_motor_encoder();
    if (current_encoder_value == last_encoder_value){
      count ++;
      if (count == 20){
        motor_encoder_reset();
        motor_set_power(0);
        return current_encoder_value;
        }
      }
    last_encoder_value = current_encoder_value;
  }
}

void power_saturation_set(uint16_t saturation){
  power_saturation = saturation;
}
