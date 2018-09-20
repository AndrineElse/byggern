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
  OLED_pos(0,0);
  // JoystickOffset joystickOffset;
  // joystickOffset = calculateOffsetJoystick();
  _delay_ms(100);
  printf('f');
  printf("Fjas");
  printf("Fjas");
  char* superfjas = "Dette var mye fjas du. Skriv noe syyyykt!";
  _delay_ms(100);
  /*for(int i= 0; i<26;i++){
    OLED_write_char('A'+i);
  }*/
  OLED_print(superfjas);

  return;
}
