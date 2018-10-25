#define F_CPU 16000000 //16 000 000 needs to be changed to atmega2560, to compile on node 2, (5000000)

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "drivers/include/UARTDriver2.h"
#include "drivers/include/SPIDriver2.h"
#include "tests/include/CANTesting2.h"
#include "tests/include/servoTesting.h"

//#define FOSC 1843200// Clock Speed
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

void main(){



  //init
  USART_Init ( MYUBRR );
  //DDRB = 0x80;
  CAN_init();
  timer_init();

  while (1) {
    testCAN();
    servo_joystick_test();
    //_delay_ms(10);
  }

  return;
}
