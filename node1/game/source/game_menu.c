#include <stdint.h>
#include "../../drivers/include/user_input_driver.h"
#include "../include/game_menu.h"
#include "../../drivers/include/OLED_driver.h"
#include <util/delay.h>

void game_menu_init(struct Node* main_menu_node,Joystick_offset offset){

  //struct Node main_menu_node;
  struct Node playGameNode;
  playGameNode.parent = main_menu_node;
  playGameNode.options[0] = "Go back";
  playGameNode.description = "Game";
  playGameNode.num_options = 1;

  struct Node highScoresNode;
  highScoresNode.parent = main_menu_node;
  highScoresNode.options[0] = "Go back";
  highScoresNode.description = "highscore";
  highScoresNode.num_options = 1;

  struct Node optionsNode;
  optionsNode.parent = main_menu_node;
  optionsNode.options[0] = "Go back";
  optionsNode.description = "options";
  optionsNode.num_options = 1;


  main_menu_node->parent = (struct Node*)0;
  main_menu_node->options[0] = "Play game";
  main_menu_node->options[1] = "Highscores";
  main_menu_node->options[2] = "Options";

  main_menu_node->description = "This is the main menu :)";
  main_menu_node->num_options = 3;

  main_menu_node->option_nodes[0] = &playGameNode;
  main_menu_node->option_nodes[1] = &highScoresNode;
  main_menu_node->option_nodes[2] = &optionsNode;

  //main_menu_node = &main_menu_node;
}

void game_menu_loop(struct Node* start_node){

  uint8_t selected_option = 0;
  Joystick_dir currentDir;
  struct Node* currentNode = start_node;
  Joystick_dir lastDir;
  lastDir = get_joystick_direction();
  uint8_t lastButtonValue = 0;
  while(1){
    //get joystick input
    Joystick_offset joystickOffset;
    joystickOffset = get_offset_joystick();
    Joystick_coords joystickCoords;
    joystickCoords = get_calibrated_joystick_coords(joystickOffset);
    Joystick_dir currentDir;
    currentDir = get_joystick_direction(joystickCoords);

    //find selected option

    if (currentDir != lastDir){
      if (currentDir == UP){
        if (selected_option > 0){
          selected_option = selected_option -1;
        }
      }
      else if (currentDir == DOWN){
        if (selected_option < (currentNode->num_options-1)){
          selected_option = selected_option +1;
        }
      }
      //evt modulo num_options her istedet
      lastDir = currentDir;
    }

    //NB, this requires selectedoptions to be < length(options)
    if (!lastButtonValue && get_joystick_button()) {
      currentNode = currentNode->option_nodes[selected_option];
      selected_option = 0;
      OLED_buffer_clear();
    }

    lastButtonValue = get_joystick_button();
    _delay_ms(50);
    print_game_node_using_buffer(currentNode, selected_option);
    OLED_buffer_update_screen();

    if currentNode->description == "Game"{
      send_joystick_position_CAN(offset);
    }
  }
}

void print_game_node(struct Node* node, uint8_t selected_option){
  OLED_clear();
  OLED_pos(0,0);
  OLED_print (node->description);

  for (int i = 0; i < node->num_options; i++){
    OLED_pos(i+1,0);
    if (i == selected_option){
      OLED_print("* ");
    }
    OLED_print(node->options[i]);

  }
  _delay_ms(500);
}

void print_game_node_using_buffer(struct Node* node, uint8_t selected_option){

  OLED_buffer_print_line (node->description,0,0);

  for (int i = 0; i < node->num_options; i++){
    if (i == selected_option){
      OLED_buffer_print_line(node->options[i], i+1, 1);
    }
    else {
      OLED_buffer_print_line(node->options[i],i+1,0);
    }
  }
}
