#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "../include/TWI_Master.h"
#include "../include/motorDriver.h"
#include "../include/servoDriver.h"


void motor_init(){
  TWI_Master_Initialise();
  TWCR |= (1 << TWIE);
}

void set_motor_speed(){
  DDRD = 0x00;
  JoystickCoords coords = get_new_joystick_values();
  unsigned char msgSize = 2;
  unsigned char msg[msgSize];
  unsigned char slave_address = 0b01011110;
  // slave_address |= (AD0 << 1) | (AD1 << 2) | (AD2 << 3) | (0101 << 4);
  printf("Slave address: %A\n\r", slave_address);

  msg[0] = slave_address;
  msg[1] = (unsigned char)coords.y;
  TWI_Start_Transceiver_With_Data(msg, msgSize);
}


// slave address for DAC:
// 0101 + AD0 + AD1 + AD2 + 0 // LSB 0 when writing to MAX520
// coords.y skal styre motorhastighet
