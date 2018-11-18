//System frequency used by util/delay, 16MHz for node 2, 5MHz for node 1
#define F_CPU 5000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "drivers/include/UARTdriver.h"
#include "drivers/include/SRAMDriver.h"
#include "tests/include/addressTesting.h"
#include "tests/include/joystickTesting.h"
#include "drivers/include/ADCDriver.h"
#include "drivers/include/OLEDDriver.h"
#include "game/include/gameMenu.h"
#include "drivers/include/userInputDriver.h"
#include "drivers/include/SPIDriver.h"
#include "drivers/include/MCP2515Driver.h"
#include "drivers/include/MCP2515.h"
#include "drivers/include/CANDriver.h"
#include "tests/include/OLEDTesting.h"
#include "tests/include/CANTesting.h"
#include "drivers/include/timerDriver.h"
#include "containers/include/gameStatusContainer.h"

#define FOSC 1843200// Clock Speed
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1




void main(){

  MCUCR = (1<<SRE);
  SFIOR = (1<<XMM2);
  //SREG |= 0x80;
  sei();
  //init
  USART_Init ( MYUBRR );


  SRAM_init ();
  SPI_init();
  mcp2515_init();
  CAN_init();



  CAN_init_interrupt();


  OLED_init();
  OLED_clear();
  OLED_init_buffer_mode();
  OLED_buffer_clear();

  //joystick_set_max_min_values();
  //printf("Sreg: %d\n\r", SREG);
  //sei();
  //printf("Sreg: %d\n\r", SREG);
  userInputInit();
  game_status_container_init();
  timer_init();
  joystick_set_max_min_values();



  menuInit();
  menuLoop();

  /*
  while(1){
    send_joystick_position();
    _delay_ms(200);
  }*/
  return;
}
