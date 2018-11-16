#define F_CPU 16000000

#include <float.h>
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include "../include/speedController.h"

// Encoder -1207 til 7480-7536 ish = ca 8700
// Joystick Y -100 to 96 = 196

volatile struct PID_data pi_container;

void speed_controller_init(float p_factor, float i_factor, float sample_time) {
  //constants
  pi_container.Kp = p_factor;
  pi_container.sample_time = sample_time;

  //variables
  pi_container.error_sum = 0;
  pi_container.current_power = 0;

  //not in use
  pi_container.encoder_factor = 0;
  pi_container.position = 0;
  pi_container.Ki = i_factor;
}


// Calculates a new power for the motor,
// should be in the range of -300, 300,
// motor saturates at -255, 255
// Params:
//  * reference_value:
//      value from -100 to 100,
//      desired speed in encoder ticks/100ms
//  * measured_value:
//      value measured from encoder,
//      conceivable values from -400 to 400
void speed_controller_calculate_power(int8_t reference_value, int16_t measured_value) {

  //TODO, Ki is probably way too big as an int.

  int16_t error = reference_value - measured_value;
  pi_container.error_sum += error;

  //returns kp*e + T*ki*int(e)
  //pi_container.current_power = (pi_container.Kp)*error + pi_container.sample_time*(pi_container.Ki)*(pi_container.error_sum);
  pi_container.current_power = (int16_t)pi_container.Kp*error;
  printf("error: %d\n\r", error);
  printf("power: %d\n\r", pi_container.current_power);
}

int16_t speed_controller_get_power() {
  //return pi_container.current_power;
  return 0;
  //return 10;
}
