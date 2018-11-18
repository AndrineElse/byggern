//system clock frequency, used by util/delay, 16MHz for node 2, 5MHz for node 1
#define F_CPU 16000000

//system libraries
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/interrupt.h>

//drivers
#include "drivers/include/UARTDriver2.h"
#include "drivers/include/CANDriver2.h"
#include "drivers/include/SPIDriver2.h"
#include "drivers/include/PWMDriver.h"
#include "drivers/include/ADCDriver2.h"
#include "drivers/include/solenoidDriver.h"

//game
#include "game/include/game.h"

//more drivers
#include "drivers/include/IRDriver.h"
#include "drivers/include/motorDriver.h"
#include "drivers/include/servoDriver.h"
#include "drivers/include/timerDriver.h"

//controller
#include "controllers/include/posController.h"

//container
#include "containers/include/userInputContainer.h"
#include "containers/include/gameDataContainer.h"



//tests
//include eventual tests here

//#define FOSC 1843200// Clock Speed
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

void main(){

  //init
  USART_Init ( MYUBRR );
  input_container_init();
  game_data_container_init();
  CAN_init();
  CAN_init_interrupt();
  pwm_init();
  adc_init();
  IR_init(5); //param: amount of samples to average for reading
  timer_hundred_ms_init();
  timer_twenty_ms_init();
  solenoid_init();
  motor_init();
  pos_controller_init(3,3,0.02,10000); //params: kp, ki, sample_time, encoder_max

  /*
  struct CAN_msg msg;
  msg.data[0] = 50;
  msg.id = 3;
  msg.length = 1;

  while(1){
    //printf("Port D : %d \n\r",(PIND));
    send_CAN_msg(&msg);
    _delay_ms(20000);
  }*/
  //printf("main\n\r");
  motor_calibrate_encoder(1);
  //game_loop();



  return;
}
