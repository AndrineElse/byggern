#define F_CPU 5000000 //16 000 000 needs to be changed to atmega2560, to compile on node 2, (5000000)

#include <avr/io.h>
#include <util/delay.h>
#include "drivers/include/UART_driver.h"
#include "drivers/include/SRAM_driver.h"
#include "tests/include/address_testing.h"
#include "tests/include/joystick_testing.h"
#include "drivers/include/ADC_driver.h"
#include "drivers/include/OLED_driver.h"
#include "drivers/include/user_input_driver.h"
#include "game/include/game_menu.h"

#include "drivers/include/SPI_driver.h"
#include "drivers/include/MCP2515_driver.h"
#include "drivers/include/MCP2515.h"
#include "drivers/include/CAN_driver.h"
#include "tests/include/OLED_testing.h"
#include "tests/include/CAN_testing.h"

#define FOSC 1843200// Clock Speed
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1




void main(){

  MCUCR = (1<<SRE);
  SFIOR = (1<<XMM2);

  //init
  USART_Init ( MYUBRR );
  //SRAM_init ();
  SPI_init();
  mcp2515_init();
  CAN_init();
  Joystick_offset offset = user_input_init();
  PORTB |= 1<<PB0; // set pinB0 as pull-up resistor input
  // OLED_test();


  struct Node main_menu_node;
  game_menu_init(&main_menu_node);
  game_menu_loop(&main_menu_node, offset);
  /*
  while(1){
    send_joystick_position_CAN(offset);
  }
  */

  return;
}
