#define F_CPU 5000000

#include <avr/io.h>
#include <util/delay.h>
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

#define FOSC 1843200// Clock Speed
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1




void main(){

  MCUCR = (1<<SRE);
  SFIOR = (1<<XMM2);

  //init
  USART_Init ( MYUBRR );
  // printf("Her 1");
  //SRAM_init ();
  SPI_init();
  mcp2515_init();
  CAN_init();

  // OLEDTest();
  testCAN();

  // SPI_SlaveInit();
  /*while(1){
    _delay_ms(2);
    printf("mcp Status: %d\n\r", mcp2515_read_status());
  }*/


  //char* superfjas = "Dette var mye fjas du. Skriv noe syyyykt!";

  /*for(int i= 0; i<26;i++){
    OLED_write_char('A'+i);
  }*/
  //OLED_print(superfjas);
/*
  while(1){
    JoystickOffset joystickOffset;
    joystickOffset = calculateOffsetJoystick();
    JoystickCoords joystickCoords;
    joystickCoords = calculateCalibratedJoystickCoords(joystickOffset);
    JoystickDir joystickDir;
    joystickDir = calculateJoystickDirection(joystickCoords);
    //printf("Joystickbutton: %d \n\r", joystickButton() );
    printf("Joystickdir : %d \n\r", joystickDir);


    joystickCoords = calculateCalibratedJoystickCoords(joystickOffset);
    printf("X = %d , Y = %d \n\r", joystickCoords.x , joystickCoords.y);
  }
*/
while(1);


  return;
}
