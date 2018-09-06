#define F_CPU 5000000

#include <avr/io.h>
#include <util/delay.h>
#include "UARTdriver.h"
#include "SRAMDriver.h"

#define FOSC 1843200// Clock Speed
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

void main(){

  //init
  USART_Init ( MYUBRR );
  SRAM_init ();

  //r/w tests
  SRAM_test();
  
  /*while(1){
    // USART_Transmit('A');
    // _delay_ms(1000);
    // printf("TEST");

  }*/
  /*
  Firkantpuls
  DDRA = 0xFF;
  PORTA = 0;
  while(1){
    PORTA = 0xFF;
    _delay_ms(10);
    PORTA = 0;
    _delay_ms(10);
  }*/
  return;
}
