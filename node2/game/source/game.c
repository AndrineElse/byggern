#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include "../../drivers/include/PIDriver.h"
#include "../../drivers/include/IRDriver.h"
#include "../../drivers/include/timerDriver.h"
#include "../../drivers/include/motorDriver.h"
#include "../../drivers/include/servoDriver.h"
#include "../../drivers/include/solenoidDriver.h"
#include "../../drivers/include/UARTDriverIoT.h"
#include "../../tests/include/servoTesting.h"


#include "../include/game.h"

// void game_init(){}

void game_loop(struct IR_status* IR_sample_container, struct PID_data* pid){
  struct Game_status game;
  game.lives = 3;
  game.fails = 0;
  game.timer = time_get_counter();
  while(game.fails < game.lives){
    servo_joystick_test();
    set_motor_speed(pid);
    solenoid_trigger();
    count_game_score(&game, IR_sample_container);
    
    //_delay_ms(10000);
  }
  game.score = time_get_counter() - game.timer;
  USART_Transmit_STXETX(game.score); // uint16_t
}


void count_game_score(struct Game_status* game,struct IR_status* IR_sample_container){
  // uint8_t last_IR_value = adc_read();
  // uint8_t count = 0;
  // while(count < game->lives){
    if (IR_poll_failure(IR_sample_container)){
      //printf("Fail registered:\n\r");
      game->fails++;
      //uint16_t pause =
      _delay_ms(2000);  //need timer like in PWM
    }
    //printf("Num fails: %d\n\r", game->fails);
  // }
}
