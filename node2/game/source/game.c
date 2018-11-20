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
#include "../include/gamePlayback.h"


#include "../include/game.h"

struct Game_status game;


void game_init() {
  game.playing = 0;
  game.running_playback = 0;
}

/*
void game_loop(){

  game.lives = 3;
  game.fails = 0;
  game.timer = time_get_counter();
  game.score = 0;
  game.playing = 0;
  game.fail_detected = 0;

  while(game.fails < game.lives){
    game.fail_detected = 0;
    game_send_update_CAN();
    //wait for node1 to give start signal
    while(!input_container_get_ptr()->playGame);

    //set intial game state
    game.timer = time_get_counter();
    solenoid_set_timer();
    game.playing = 1;
    game.fail_detected = 0;
    //solenoid_set_timer();
    solenoid_reset();
    //housekeeping before game start
    IR_reset_samples();
    pos_controller_reset();

    //play until you die
    while(game.playing){

      //these functions are currently run in the respective ISRs
      //for their sampled values. Only running if game.playing is high
      //IR_get_new_sample();
      //solenoid_update_status(input_container_get_ptr()->joystickButton);
      if (IR_check_obstruction()){
        //cli();
        game.fail_detected = 1;
        solenoid_reset();
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
*/

uint8_t game_get_playing_status() {
  return game.playing;
}

uint8_t game_get_playback_status() {
  return game.running_playback;
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
  uint8_t array[8] = {((game.running_playback)+(game.fail_detected << 6)+(game.fails << 3)+(game.lives)),((game.score & 0xFF00) >> 8),(game.score & 0x00FF),0,0,0,0,0};
  for (int j = 0; j < 8; j++){
    msg.data[j] = array[j];
  }
  msg.length = 3;
  cli();
  send_CAN_msg(&msg);
  sei();
}


void game_loop(){
  while (1) {
    game_init();
    //add switch for selecting mode here
    while(input_container_get_ptr()->run_playback);

    if(!(input_container_get_ptr()->restart_game)){

        game.lives = 3;
        game.fails = 0;
        game.timer = time_get_counter();
        game.score = 0;
        game.playing = 0;
        game.running_playback = 0;
        game.fail_detected = 0;

        while(game.fails < game.lives){
          game.fail_detected = 0;
          game_send_update_CAN();

          //wait for new desired state from node1
          while(!(input_container_get_ptr()->playGame || input_container_get_ptr()->run_playback)) {
            printf("B\n\r");
          }

          if(input_container_get_ptr()->run_playback){
            printf("C\n\r");
            motor_get_max_encoder();//used to reset encoder, and drive to known location
            printf("D\n\r");
            pos_controller_reset();//reset error sum, and latest power.
            printf("D\n\r");

            game.running_playback = 1;
            game_send_update_CAN();
            printf("%d %d\n\r",!playback_get_finished_playing(),input_container_get_ptr()->run_playback );
            while(input_container_get_ptr()->run_playback && !playback_get_finished_playing()){
              printf("a\n\r");
              _delay_ms(10);
            }
          }
          printf("E\n\r");
          game.running_playback = 0;
          playback_stop_playing();

          game_send_update_CAN();

          //wait for node1 to give start signal
          while(!input_container_get_ptr()->playGame);

          //TODO, move function calls up, and assignments down
          //test solenoid afterwards

          //set intial game state
          game.timer = time_get_counter();
          solenoid_set_timer();
          game.playing = 1;
          solenoid_reset();

          //housekeeping before game start
          IR_reset_samples();
          pos_controller_reset();

          //play until you lose the ball
          while(game.playing){

            //while game.playing is high:
            //  upon receiving a new can message:
            //    * solenoid updated from user input (limited by timer)
            //    * servo updated from user input
            //  every 20ms:
            //    * new controller power computed and applied every 20ms
            //    * new IR value sampled every 20ms

            if (IR_check_obstruction()){
              //cli();
              game.fail_detected = 1;
              solenoid_reset();
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
          while(input_container_get_ptr()->playGame) {
            printf("noDed\n\r");
          }
        }
    }
  }
}
