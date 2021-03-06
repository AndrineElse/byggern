#define F_CPU 16000000

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include "../../controllers/include/posController.h"
#include "../../drivers/include/IRDriver.h"
#include "../../drivers/include/timerDriver.h"
#include "../../drivers/include/motorDriver.h"
#include "../../drivers/include/CANDriver2.h"
#include "../../drivers/include/servoDriver.h"
#include "../../drivers/include/solenoidDriver.h"
#include "../../tests/include/servoTesting.h"
#include "../../containers/include/userInputContainer.h"

#include "../include/game.h"

void game_loop(){

  struct Game_status game;
  game.lives = 3;
  game.fails = 0;
  game.timer = time_get_counter();

  uint8_t button_flag = 0;
  uint16_t solenoid_timer = 0;

  while(game.fails < game.lives){

    servo_update_position(input_container_get_ptr()->joystick.x);
    motor_set_power(pos_controller_get_power());
    solenoid_update_status(&button_flag, &solenoid_timer);
    count_game_score(&game);
    //printf("I:%d\n\r",IR_get_mean_value());
  }

  game.score = time_get_counter() - game.timer;
}


void count_game_score(struct Game_status* game){
  if (IR_check_obstruction()){
    game->fails++;
    _delay_ms(2000);  //need timer like in PWM
  }
}

/*
MAPPING
id = 2 = IR-Status
data[0] = game.lives
data[1] = game.fails
data[3] = game.timer
data[4] = game.score
*/

void game_send_update_CAN(struct Game_status* game){
  struct CAN_msg msg;
  msg.id = 2;
  uint8_t array[8] = {game->lives,game->fails,game->timer,game->score,0,0,0};

  for (int j = 0; j < 8; j++){
    msg.data[j] = array[j];

  }
  msg.length = 4;
  send_CAN_msg(&msg);
}
