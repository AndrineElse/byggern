#define F_CPU 5000000

#include <avr/io.h>
#include <util/delay.h>
#include "drivers/include/UARTdriver.h"
#include "drivers/include/SRAMDriver.h"
#include "tests/include/addressTesting.h"
#include "tests/include/joystickTesting.h"
#include "drivers/include/ADCDriver.h"
#include "drivers/include/OLEDDriver.h"

#define FOSC 1843200// Clock Speed
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

void main(){
  MCUCR = (1<<SRE);
  SFIOR = (1<<XMM2);
  OLED_clear();

  //init
  USART_Init ( MYUBRR );
  //SRAM_init ();
  OLED_init();
  // JoystickOffset joystickOffset;
  // joystickOffset = calculateOffsetJoystick();
  printf("Fjas");
  printf("Fjas");
  printf("Fjas");
  OLED_pos(0,0);
  _delay_ms(100);
  for(int i= 0; i<26;i++){
    OLED_write_char('A'+i);
  }
  


  // OLED_write_command(0xa5);


  /*while(1){
    OLED_write_data()
    OLED_write_command(0xa5);
    _delay_ms(100);
/*
    SliderPosition sampleSlider;
    JoystickCoords sampleJoystick;
    sampleJoystick = calculateCalibratedJoystickCoords(joystickOffset);
    sampleSlider = calculateSliderPosition();
    printf("\033[F");
    printf("\033[K");
    printf("\033[F");
    printf("\033[K");
    printf("\033[F");
    printf("\033[K");
    printf("Left: %d\n\r",sampleSlider.left);
    printf("Right: %d\n\r",sampleSlider.right);
    printf("---------------------");
  }*/


  //joystickTest();
  /*while(1){
    addressTest();
  }*/

  //r/w tests
  //SRAM_test();

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
