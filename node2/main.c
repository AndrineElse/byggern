#define F_CPU 16000000 //16 000 000 needs to be changed to atmega2560, to compile on node 2, (5000000)

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "drivers/include/UARTDriver2.h"
#include "drivers/include/SPIDriver2.h"
#include "drivers/include/ADCDriver2.h"
#include "drivers/include/PWMDriver.h"
#include "tests/include/CANTesting2.h"
#include "tests/include/servoTesting.h"
#include "tests/include/ADCTesting.h"
#include "tests/include/gameTesting.h"
#include "drivers/include/IRDriver.h"
//#define FOSC 1843200// Clock Speed
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

volatile uint8_t counter;

ISR(TIM3_COMPA_vect) {
  counter++;
}

void main(){
  counter = 0;


  //init
  USART_Init ( MYUBRR );
  /*
  CAN_init();
  pwm_init();
  adc_init();
  struct IR_status IR_sample_container;
  IR_init(&IR_sample_container);
  */
  timer_init();


  //tests
  //game_test(&IR_sample_container);

  while (1) {
    sei();
    printf("OCR: %d\n\r",OCR3A);
    printf("CNT: %d\n\r",TCNT3);
    printf("counter: %d\n\r",counter);
    //_delay_ms(100);
  }

  return;
}
