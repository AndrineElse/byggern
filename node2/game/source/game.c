#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include "../../drivers/include/IRDriver.h"
#include "../../drivers/include/timerDriver.h"
#include "../../drivers/include/motorDriver.h"
#include "../../containers/include/userInputContainer.h"
#include "../include/game.h"

// void game_init(){}
volatile struct Input_status input_container;

void game_loop(){
  struct Game_status game;
  game.lives = 3;
  game.fails = 0;
  game.timer = time_get_counter();

  while(game.fails < game.lives){
    update_servo_position(get_input_ptr()->joystick);
    set_motor_speed();
    count_game_score(&game);
    _delay_ms(10000);
  }
  game.score = time_get_counter() - game.timer;
}


void count_game_score(struct Game_status* game){
  if (IR_poll_failure()){
    printf("Fail registered:\n\r");
    
    game->fails++;
    IR_reset_samples();
    //implement timer delay here!
    _delay_ms(2000); 
  }
  printf("Num fails: %d\n\r", game->fails);
}