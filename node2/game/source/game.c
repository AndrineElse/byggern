#define F_CPU 16000000

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
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

void game_loop(struct IR_status* IR_sample_container){

  struct Game_status game;
  game.lives = 3;
  game.fails = 0;
  game.timer = time_get_counter();

  uint8_t button_flag = 0;
  uint16_t solenoid_timer = 0;
  uint16_t update_CAN_timer=0;
  uint8_t update_CAN_flag=0;
  while(game.fails < game.lives){

    servo_update_position(input_container_get_ptr()->joystick.x);
    pos_controller_update();
    motor_set_power(pos_controller_get_power());
    solenoid_update_status(&button_flag,&solenoid_timer);
    count_game_score(&game, IR_sample_container);
    _delay_ms(1000);
    game_send_update_CAN(&game,&update_CAN_timer,&update_CAN_flag);

  }

  game.score = time_get_counter() - game.timer;
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
    printf("Num fails: %d\n\r", game->fails);
  // }
}

/*
MAPPING
id = 2 = IR-Status
data[0] = game.timer
data[1] = game.fails
data[2] = game.lives
data[3] = game.score
*/

void game_send_update_CAN(struct Game_status* game, uint16_t* timer, uint8_t* flag){
  if(*flag ==0){
    *timer = time_get_counter();
    *flag = 1;
  }
  else{
    if((time_get_counter() - *timer) > 10){
      struct CAN_msg msg;
      msg.id = 2;
      uint8_t array[8] = {game->timer,game->fails,game->lives,game->score,0,0,0};

      for (int j = 0; j < 8; j++){
        msg.data[j] = array[j];

      }
      msg.length = 4;
      cli();
      send_CAN_msg(&msg);
      sei();
      *flag = 0;
      *timer = 0;
    }
  }
}


void game_select_controller(struct CAN_msg new_input_message){
  uint8_t level = new_input_message.data[0]; // 0 = easy, 1 = medium, 2 = hard
  switch(level){
    case 0:   // easy
      pos_controller_update(3,3);
    case 1:   // medium
      pos_controller_update(1,1);
    case 2:   // hard
      pos_controller_update(1,1);
    default:
      pos_controller_update(3,3);
  }
}
