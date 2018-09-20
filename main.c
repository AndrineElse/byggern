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

#define FOSC 1843200// Clock Speed
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

Node mainMenuNode = {.parent=((Node*)0), .options={"Play game","Highscores", "Options"}, .optionNodes= ((Node**){0,0,0}), .description = "This is the main menu :)", .numOptions = 3};

Node playGameNode = {&mainMenuNode, {"Coming soon"} ,0, "Game", 1 };

Node highScoresNode = {&mainMenuNode, {"Coming soon"} ,0, "highscore", 1};

Node optionsNode = {&mainMenuNode, {"Coming soon"} ,0, "options", 1 };

mainMenuNode.optionNodes[0] = &playGameNode;
mainMenuNode.optionNodes[1] = &highScoresNode;
mainMenuNode.optionNodes[2] = &optionsNode;

void main(){
  MCUCR = (1<<SRE);
  SFIOR = (1<<XMM2);
  OLED_clear();

  //init
  USART_Init ( MYUBRR );
  //SRAM_init ();
  OLED_init();
  OLED_pos(0,6);
  // JoystickOffset joystickOffset;
  // joystickOffset = calculateOffsetJoystick();

  //char* superfjas = "Dette var mye fjas du. Skriv noe syyyykt!";

  /*for(int i= 0; i<26;i++){
    OLED_write_char('A'+i);
  }*/
  //OLED_print(superfjas);

  menuLoop(mainMenuNode);

  return;
}
