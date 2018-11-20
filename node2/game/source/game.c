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
#include "../../containers/include/userInputContainer.h"
#include "../include/gamePlayback.h"
#include "../include/game.h"

struct Game_status game;

void game_init() {
  game.playing = 0;
  game.running_playback = 0;
}

void game_loop(){
  while (1) {
    game_init();

    if(!(input_container_get_ptr()->restart_game)){

      game.lives = 3;
      game.fails = 0;
      game.timer = time_get_counter();
      game.score = 0;
      game.playing = 0;
      game.fail_detected = 0;
      game.running_playback = 0;

      while(game.fails < game.lives){
        game.fail_detected = 0;
        game_send_update_CAN();

        //wait for new desired state from node1
        while(!(input_container_get_ptr()->playGame || input_container_get_ptr()->run_playback));

        if(input_container_get_ptr()->run_playback){

          //houskeeeping
          motor_get_max_encoder();
          pos_controller_reset();

          //letting node1 playback has started
          game.running_playback = 1;
          game_send_update_CAN();


          //waiting for either a stop from node1, or for the playback to run out of samples
          while(input_container_get_ptr()->run_playback && !playback_get_finished_playing()){
            _delay_ms(10);
          }


          //housekeeping after finishing a playback
          game.running_playback = 0;
          playback_stop_playing();
          game_send_update_CAN();

          //waiting for node1 to ackowledge playback has stopped
          while(input_container_get_ptr()->run_playback){
            _delay_ms(10);
          }
        } else if (input_container_get_ptr()->playGame) {
          //set intial game state
          game.timer = time_get_counter();
          solenoid_set_timer();
          game.playing = 1;
          solenoid_reset();
          IR_reset_samples();
          pos_controller_reset();

          //play until you die
          while(game.playing){
            //these functions are currently run in the respective ISRs
            //for their sampled values. Only running if game.playing is high
            //IR_get_new_sample();
            //solenoid_update_status(input_container_get_ptr()->joystickButton);
            if (IR_check_obstruction()){
              game.fail_detected = 1;
              solenoid_reset();
              game.fails++;
              game.playing = 0;
              motor_set_power(0);
              servo_update_position(0);
              game.score += (time_get_counter()-game.timer);
              game_send_update_CAN();
              break;
            }
          }

          //waiting for node1 to acknowledge death
          while(input_container_get_ptr()->playGame);
        }
      }
    }
  }
}

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
  uint8_t array[8] = {((game.running_playback << 7) + (game.fail_detected << 6) + (game.fails << 3) + (game.lives)),
                      ((game.score & 0xFF00) >> 8),
                      (game.score & 0x00FF),
                      0,0,0,0,0};
  for (int j = 0; j < 8; j++){
    msg.data[j] = array[j];
  }
  msg.length = 3;
  cli();
  send_CAN_msg(&msg);
  sei();
}

void game_select_controller(struct CAN_msg new_input_message){
  uint8_t level = new_input_message.data[0];
  switch(level){
    case 0:   // easy
      pos_controller_update(1,5);
      power_saturation_set(80);
      break;
    case 1:   // medium
      pos_controller_update(1,5);
      power_saturation_set(100);
      break;
    case 2:   // hard
      pos_controller_update(1,5);
      power_saturation_set(150);
      break;
  }
}
