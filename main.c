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

#define FOSC 1843200// Clock Speed
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1




void main(){

  struct Node mainMenuNode;

  struct Node playGameNode = {&mainMenuNode, {"Coming soon"} ,((struct Node**){0}), "Game", 1 };

  struct Node highScoresNode = {&mainMenuNode, {"Coming soon"} ,((struct Node**){0}), "highscore", 1};

  struct Node optionsNode = {&mainMenuNode, {"Coming soon"} ,((struct Node**){0}), "options", 1 };

  mainMenuNode.parent = (struct Node*)0;
  mainMenuNode.options[0] = "Play game";
  mainMenuNode.options[1] = "Highscores";
  mainMenuNode.options[2] = "Options";

  mainMenuNode.description = "This is the main menu :)";
  mainMenuNode.numOptions = 3;

  mainMenuNode.optionNodes[0] = &playGameNode;
  mainMenuNode.optionNodes[1] = &highScoresNode;
  mainMenuNode.optionNodes[2] = &optionsNode;

  MCUCR = (1<<SRE);
  SFIOR = (1<<XMM2);
  PORTB = 0x80; //set PB7 as input



  //init
  USART_Init ( MYUBRR );
  // printf("Her 1");
  //SRAM_init ();
  OLED_init();
  OLED_clear();
  _delay_ms(1000);
  OLED_fill();
  _delay_ms(1000);
  OLED_clear();
  OLED_pos(0,0);
  printf("Her 2");


  JoystickOffset joystickOffset;
  joystickOffset = calculateOffsetJoystick();
  JoystickCoords joystickCoords;


  //char* superfjas = "Dette var mye fjas du. Skriv noe syyyykt!";

  /*for(int i= 0; i<26;i++){
    OLED_write_char('A'+i);
  }*/
  //OLED_print(superfjas);
/*
  while(1){
    joystickCoords = calculateCalibratedJoystickCoords(joystickOffset);
    printf("X = %d , Y = %d \n\r", joystickCoords.x , joystickCoords.y);
  }
*/
  menuLoop(&mainMenuNode);

  return;
}
