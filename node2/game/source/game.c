#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include "../../drivers/include/IRDriver.h"

#include "../include/game.h"

// void game_init(){}

void game_loop(struct IR_status* IR_sample_container){
  struct Game_status game;
  game.lives = 3;
  game.fails = 0;
  while(game.fails < game.lives){
    servo_joystick_test();
    count_game_score(&game, IR_sample_container);
  }
}


void count_game_score(struct Game_status* game,struct IR_status* IR_sample_container){
  // uint8_t last_IR_value = adc_read();
  // uint8_t count = 0;
  // while(count < game->lives){
    if (IR_poll_failure(IR_sample_container)){
      printf("Fail registered:\n\r");
      game->fails++;
      _delay_ms(1000);  //need timer like in PWM
    }
    printf("Num fails: %d\n\r", game->fails);
  // }
}
