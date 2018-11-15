#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include "../../drivers/include/PIDriver.h"
#include "../../drivers/include/IRDriver.h"
#include "../../drivers/include/timerDriver.h"
#include "../../drivers/include/motorDriver.h"
#include "../../drivers/include/CANDriver2.h"
#include "../../drivers/include/servoDriver.h"
#include "../../drivers/include/solenoidDriver.h"
#include "../../tests/include/servoTesting.h"
#include "../../containers/include/userInputContainer.h"

#include "../include/game.h"

// void game_init(){}

void game_loop(struct IR_status* IR_sample_container, struct PID_data* pid){
  struct Game_status game;
  game.lives = 3;
  game.fails = 0;
  game.timer = time_get_counter();
  uint8_t button_flag = 0;
  uint16_t solenoid_timer = 0;
  while(game.fails < game.lives){
    servo_update_position(input_container_get_ptr()->joystick.x);
    set_motor_speed(pid);
    solenoid_update_status(&button_flag,&solenoid_timer);
    game_send_update_CAN(&game);
    count_game_score(&game, IR_sample_container);
    //_delay_ms(10000);
  }
  game.score = time_get_counter() - game.timer;
}


void count_game_score(struct Game_status* game,struct IR_status* IR_sample_container){
  if (IR_poll_failure(IR_sample_container)){
    game->fails++;
    _delay_ms(2000);  //need timer like in PWM
  }
}

/*
MAPPING
id = 2 = IR-Status
data[0] = game.fails
data[1] = game.timer
data[2] = game.lives
data[3] = game.score
*/

void game_send_update_CAN(struct Game_status* game){
  struct CAN_msg msg;
  msg.id = 2;
  uint8_t array[8] = {game->fails,game->timer,game->lives,game->score,0,0,0};

  for (int j = 0; j < 8; j++){
    msg.data[j] = array[j];

  }
  msg.length = 4;
  send_CAN_msg(&msg);
}
