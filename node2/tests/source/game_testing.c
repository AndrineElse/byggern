#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include "../../drivers/include/IR_driver.h"
#include "../../game/include/game.h"
#include "../include/game_testing.h"
#include "../include/servo_testing.h"


void game_test(struct IR_status* IR_sample_container){
  game_loop(IR_sample_container);
  /*struct Game_status game;
  game.lives = 3;
  count_game_score(&game, IR_sample_container);*/
}
