#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include "../../drivers/include/IRDriver.h"

#include "../include/game.h"




void count_game_score(struct Game_status* game,struct IR_status* IR_sample_container){
  uint8_t last_IR_value = adc_read();
  uint8_t count = 0;
  while(count < game->lives){
    if (IR_poll_failure(&last_IR_value,IR_sample_container)){
      printf("Fail registered:\n\r");
      count++;
      game->fails = count;
      _delay_ms(1000);  //need timer like in PWM
    }
    printf("Num fails: %d\n\r", game->fails);
  }
}
