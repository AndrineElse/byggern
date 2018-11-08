#include <avr/io.h>
#include <util/delay.h>
#include "../../drivers/include/OLED_driver.h"
#include <avr/pgmspace.h>
#include "../../drivers/include/UART_driver.h"
#include <stdint.h>
#include "../include/OLED_testing.h"
#include "../../game/include/game_menu.h"



void OLED_test(){
  PORTB = 0x01; //set PB1 as input

  struct Node main_menu_node;
  // game_menu_init(&main_menu_node);

  struct Node playGameNode;
  playGameNode.parent = &main_menu_node;
  playGameNode.options[0] = "Go back";
  playGameNode.option_nodes[0] = playGameNode.parent;
  playGameNode.description = "Game";
  playGameNode.num_options = 1;

  struct Node highScoresNode;
  highScoresNode.parent = &main_menu_node;
  highScoresNode.options[0] = "Go back";
  highScoresNode.option_nodes[0] = highScoresNode.parent;
  highScoresNode.description = "highscore";
  highScoresNode.num_options = 1;

  struct Node optionsNode;
  optionsNode.parent = &main_menu_node;
  optionsNode.options[0] = "Go back";
  optionsNode.option_nodes[0] = optionsNode.parent;
  optionsNode.description = "options";
  optionsNode.num_options = 1;


  main_menu_node.parent = (struct Node*)0;
  main_menu_node.options[0] = "Play game";
  main_menu_node.options[1] = "Highscores";
  main_menu_node.options[2] = "Options";

  main_menu_node.description = "This is the main menu :)";
  main_menu_node.num_options = 3;

  main_menu_node.option_nodes[0] = &playGameNode;
  main_menu_node.option_nodes[1] = &highScoresNode;
  main_menu_node.option_nodes[2] = &optionsNode;




  OLED_init();
  OLED_clear();
  _delay_ms(1000);
  OLED_fill();
  _delay_ms(1000);

  OLED_init_buffer_mode();
  OLED_buffer_clear();
  OLED_buffer_update_screen();
  _delay_ms(1000);

  game_menu_loop(&main_menu_node);

}
