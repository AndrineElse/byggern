#define F_CPU 16000000

#include <stdio.h>
#include <float.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "../include/TWI_Master.h"
#include "../include/motorDriver.h"
#include "../include/servoDriver.h"

int16_t min_encoder;
int16_t max_encoder;

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
  motor_set_power(0);
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


void motor_encoder_reset(){
    PORTH &= ~(1<<PH6);
    _delay_us(10);
    PORTH |= 1<<PH6;
}

void motor_set_max_min_encoder(uint8_t dir){
  motor_encoder_reset();
  int16_t last_encoder_value = 100;
  int16_t current_encoder_value;
  uint8_t count = 0;
  int8_t k = 1;
  if (dir){
    k = -1;
  }
  //Drive the motor to the opposite side
  while(count < 5){
    current_encoder_value = read_motor_encoder();
    printf("Current encoder: %d\n", current_encoder_value);
    motor_set_power((-1)*k*(60));
    if (current_encoder_value == last_encoder_value){
      count ++;
    }
    last_encoder_value = current_encoder_value;
  }
  motor_set_power(0);
  count = 0;
  while(count < 5){
    current_encoder_value = read_motor_encoder();
    printf("Current encoder: %d\n", current_encoder_value);
    motor_set_power(k*(60));
    if (current_encoder_value == last_encoder_value){
      count ++;
      if (count == 10){
        motor_set_power(0);
        switch (dir) {
          case 1:
            min_encoder  = current_encoder_value;
            printf("Min value : %d \n", min_encoder );
            break;
          case 0:
            max_encoder = (-1)*current_encoder_value;
            printf("Max value : %d \n", max_encoder );
            break;
        }
        break;
      }
    }
    last_encoder_value = current_encoder_value;
  }
}

void motor_get_max_min_encoder_ptr(int8_t* min, int8_t* max){
  *min = min_encoder;
  *max = max_encoder;
}
