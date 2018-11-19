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

struct Game_status game;


void game_init() {
  game.playing = 0;
}

void game_loop(){

  game.lives = 3;
  game.fails = 0;
  game.timer = time_get_counter();
  game.score = 0;
  game.playing = 0;
  game.fail_detected = 0;

  while(game.fails < game.lives){

    //wait for node1 to give start signal
    while(!input_container_get_ptr()->playGame);

    //set intial game state
    game.timer = time_get_counter();
    game.playing = 1;
    game.fail_detected = 0;

    //housekeeping before game start
    IR_reset_samples();
    pos_controller_reset();

    //play until you die
    while(game.playing){

      //these functions are currently run in the respective ISRs
      //for their sampled values. Only running if game.playing is high
      /*
      servo_update_position(input_container_get_ptr()->joystick.x);
      motor_set_power(pos_controller_get_power());
      solenoid_update_status(input_container_get_ptr()->joystickButton);
      */
      IR_get_new_sample();
      solenoid_update_status(input_container_get_ptr()->joystickButton);

      if (IR_check_obstruction()){
        //cli();
        game.fail_detected = 1;
        game.fails++;
        game.playing = 0;
        motor_set_power(0);
        servo_update_position(0);
        game.score += (time_get_counter()-game.timer);
        game_send_update_CAN();
        //sei();
        break;
      }
    }

    //waiting for node1 to acknowledge death
    while(input_container_get_ptr()->playGame);
  }
  //state: game is over, node1 has acked and received final score.
}

uint8_t game_get_playing_status() {
  return game.playing;
}


void count_game_score(){
  if (IR_check_obstruction()){
    game.fails++;
    game.playing = 0;
    game_send_update_CAN();
  }
}

// sends every half second while game is running,
// and one extraordinary message on death
void game_send_update_CAN(){
  struct CAN_msg msg;
  msg.id = 2;

  //NB need to add game-score here !!
  uint8_t array[8] = {((game.fail_detected << 6)+(game.fails << 3)+(game.lives)),0,0,0,0,0,0,0};

  for (int j = 0; j < 8; j++){
    msg.data[j] = array[j];

  }
  msg.length = 1;
  cli();
  send_CAN_msg(&msg);
  sei();
}
