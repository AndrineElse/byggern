#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include "../../drivers/include/IRDriver.h"
#include "../../game/include/game.h"
#include "../include/gameTesting.h"


void game_test(struct IR_status* IR_sample_container){
  struct Game_status game;
  game.lives = 3;
  count_game_score(&game, IR_sample_container);
}
