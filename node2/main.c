#define F_CPU 16000000 //16 000 000 needs to be changed to atmega2560, to compile on node 2, (5000000)

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "drivers/include/UARTDriver2.h"
#include "drivers/include/SPIDriver2.h"
#include "tests/include/CANTesting2.h"
#include "drivers/include/PWMDriver.h"
#include "tests/include/servoTesting.h"
#include "drivers/include/ADCDriver2.h"
#include "tests/include/ADCTesting.h"
#include "game/include/game.h"
#include "drivers/include/IRDriver.h"
#include "drivers/include/motorDriver.h"
#include "drivers/include/servoDriver.h"
#include "drivers/include/PIDriver.h"

//#define FOSC 1843200// Clock Speed
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

void main(){


  //init
  USART_Init ( MYUBRR );

  CAN_init();
  //timer_init();
  pwm_init();
  adc_init();
  struct IR_status IR_sample_container;
  IR_init(&IR_sample_container);

  timer_init();

  struct PID_data pid;
  motor_init(&pid);
  game_loop(&IR_sample_container, &pid);
  while (1) {
    // servo_test();
    adc_test();

    _delay_ms(1000);
  }




  return;
}
