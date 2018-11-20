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


//#define FOSC 1843200// Clock Speed
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

void main(){
  game_init();
  //init
  USART_Init ( MYUBRR );
  input_container_init();
  CAN_init();
  CAN_init_interrupt();
  pwm_init();
  adc_init();
  IR_init(10); //param: amount of samples to average for reading
  timer_hundred_ms_init();
  timer_twenty_ms_init();
  solenoid_init();
  motor_init();
  pos_controller_init(1,5,0.02); //params: kp, ki, sample_time, encoder_max

  game_loop();

  return;
}
