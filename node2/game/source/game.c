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
#include "../../containers/include/gameDataContainer.h"

#include "../include/game.h"

void game_loop(){

  struct Game_status game;
  game.lives = 3;
  game.fails = 0;
  game.timer = time_get_counter();
  game.score = 0;
  uint8_t button_flag = 0;
  uint16_t solenoid_timer = 0;
  uint16_t update_CAN_timer=0;
  uint8_t update_CAN_flag=0;
  uint8_t fail_registerd_flag =0;
  uint16_t fail_timer=0;
  while(game.fails < game.lives){


    if(input_container_get_ptr()->playGame){

      IR_get_new_sample();

      servo_update_position(input_container_get_ptr()->joystick.x);
      motor_set_power(pos_controller_get_power());
      solenoid_update_status(&button_flag,&solenoid_timer);
      //count_game_score(&game,&fail_timer,&fail_registerd_flag);
      count_game_score(&game);
      //_delay_ms(1000);
      game.score = time_get_counter() - game.timer;
      game_send_update_CAN(&game,&update_CAN_timer,&update_CAN_flag);

    }
    else{
      motor_set_power(0);
      game_send_update_CAN(&game,&update_CAN_timer,&update_CAN_flag);
      solenoid_update_status(0,&solenoid_timer);
    }
  }

  //game.score = time_get_counter() - game.timer;
}



/*
void count_game_score(struct Game_status* game, uint16_t* timer, uint8_t* flag){
  // uint8_t last_IR_value = adc_read();
  // uint8_t count = 0;
  // while(count < game->lives){
    if(*flag ==0){
      if (IR_check_obstruction()){
        //printf("Fail registered:\n\r");
        game->fails++;
        printf("Fails++ %d\n\r", game->fails);
        //uint16_t pause =
         //need timer like in PWM
         *timer = time_get_counter();
         *flag = 1;
      }
      else{
        if((time_get_counter() - *timer) > 10){
          *timer = 0;
          *flag = 0;
        }
      }
    }

}*/


void count_game_score(struct Game_status* game){
  if (IR_check_obstruction()){
    //printf("Fail registered:\n\r");
    game->fails++;
    printf("Fails++ %d\n\r", game->fails);
    //uint16_t pause =
     //need timer like in PWM

  }
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
    if((time_get_counter() - *timer) > 5){
      struct CAN_msg msg;
      msg.id = 2;
      //printf("Fails = %d\n\r",game->fails);
      uint8_t array[8] = {((game->fails << 4)+game->lives),0,0,0,0,0,0,0}; //NB need to add game-score here !!

      for (int j = 0; j < 8; j++){
        msg.data[j] = array[j];

      }
      printf("Fails = %d\n\r",((msg.data[0] & 0xF0)>>4));
      printf("Lives = %d\n\r", (msg.data[0] & 0x0F));
      msg.length = 1;
      cli();
      send_CAN_msg(&msg);
      sei();
      *flag = 0;
      *timer = 0;
    }
  }
}
