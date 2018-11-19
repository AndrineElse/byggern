//System frequency used by util/delay, 16MHz for node 2, 5MHz for node 1
#define F_CPU 5000000

#include <avr/io.h>
#include <util/delay.h>
#include "../../drivers/include/OLEDDriver.h"
#include <avr/pgmspace.h>
#include "../../drivers/include/UARTdriver.h"
#include <stdint.h>
#include "../include/OLEDTesting.h"
#include "../../game/include/gameMenu.h"



void OLEDTest(){
  PORTB = 0x01; //set PB1 as input

  struct Node mainMenuNode;
  // menuInit(&mainMenuNode);

  struct Node playGameNode;
  playGameNode.parent = &mainMenuNode;
  playGameNode.options[0] = "Go back";
  playGameNode.optionNodes[0] = playGameNode.parent;
  playGameNode.description = "Game";
  playGameNode.numOptions = 1;

  struct Node highScoresNode;
  highScoresNode.parent = &mainMenuNode;
  highScoresNode.options[0] = "Go back";
  highScoresNode.optionNodes[0] = highScoresNode.parent;
  highScoresNode.description = "highscore";
  highScoresNode.numOptions = 1;

  struct Node optionsNode;
  optionsNode.parent = &mainMenuNode;
  optionsNode.options[0] = "Go back";
  optionsNode.optionNodes[0] = optionsNode.parent;
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




  OLED_init();
  OLED_buffer_clear();
  _delay_ms(1000);
  OLED_buffer_fill();
  _delay_ms(1000);

  OLED_init_buffer_mode();
  OLED_buffer_clear();
  OLED_buffer_update_screen();
  _delay_ms(1000);

  menuLoop(&mainMenuNode);

}
