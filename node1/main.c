#define F_CPU 5000000 //16 000 000 needs to be changed to atmega2560, to compile on node 2, (5000000)

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
#include "containers/include/gameStatusContainer.h"

#define FOSC 1843200// Clock Speed
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1




void main(){

  MCUCR = (1<<SRE);
  SFIOR = (1<<XMM2);
  SREG |= 0x80;

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
  game_status_container_init();
  JoystickOffset offset = userInputInit();
  timer_init();

  //OLEDTest();
  /*
  while (1) {

    struct CAN_msg received_message = receive_msg();
    printf("Data [0]: %d , ID = %d\n\r", received_message.data[0], received_message.id);
    if (received_message.id == 3){ //Because joystick messages has id 1
      printf("HER\n");
      printf("Data [1]\n\r", received_message.data[0]);
    }
    //send_joystick_position(offset);
  }
  */
  /*
  while(1){
    printf("%d\n\r",timer_get_counter());
    _delay_ms(1000);
  }
  */
  struct Node mainMenuNode;
  //struct Node mainMenuNode;
  struct Node playGameNode;
  playGameNode.parent = &mainMenuNode;
  playGameNode.options[0] = "Go back";
  playGameNode.description = "Game";
  playGameNode.numOptions = 1;

  struct Node highScoresNode;
  highScoresNode.parent = &mainMenuNode;
  highScoresNode.options[0] = "Go back";
  highScoresNode.description = "highscore";
  highScoresNode.numOptions = 1;

  struct Node optionsNode;
  optionsNode.parent = &mainMenuNode;
  optionsNode.options[0] = "Go back";
  optionsNode.description = "options";
  optionsNode.numOptions = 1;


  mainMenuNode.parent = (struct Node*)0;
  mainMenuNode.options[0] = "Play game";
  mainMenuNode.options[1] = "Highscores";
  mainMenuNode.options[2] = "Options";

  mainMenuNode.description = "This is the main menu :)";
  mainMenuNode.numOptions = 3;

  mainMenuNode.optionNodes[0] = &playGameNode;
  mainMenuNode.optionNodes[1] = &highScoresNode;
  mainMenuNode.optionNodes[2] = &optionsNode;
  menuLoop(&mainMenuNode);

  /*
  while(1){
    send_joystick_position(offset);
  }
*/



  // SPI_SlaveInit();
  /*while(1){
    _delay_ms(2);
    printf("mcp Status: %d\n\r", mcp2515_read_status());
  }*/

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
  //while(1);


  return;
}
