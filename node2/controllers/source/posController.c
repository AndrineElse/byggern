#define F_CPU 16000000

#include <float.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "../../drivers/include/CANDriver2.h"
#include "../include/posController.h"
#include "../../containers/include/userInputContainer.h"

// Encoder -1207 til 7480-7536 ish = ca 8700
// Joystick Y -100 to 96 = 196

volatile struct PID_data pi_container;


void pos_controller_init(int8_t p_factor, int8_t i_factor, float sample_time, uint16_t encoder_max) {
  //constants
  pi_container.Kp = p_factor;
  pi_container.Ki = i_factor;
  pi_container.sample_time = sample_time;
  pi_container.encoder_max = encoder_max;
  pi_container.encoder_scale = 39;

  //variables
  pi_container.error_sum = 0;
  pi_container.current_power = 0;
  pi_container.last_encoder_value = 0;
}


// Calculates a new power for the motor,
// should be in the range of -300, 300,
// motor saturates at -255, 255
// Params:
//    reference_value:
//      value from 0 to 255,
//        0 being left wall,
//        255 being right wall
//    reference_value:
//      value from 0 to 11000 ish
//      should probably be saturated from 0 to 110000
void pos_controller_calculate_power(uint8_t reference_value, int16_t measured_value) {
  uint16_t scaled_reference = reference_value*pi_container.encoder_scale;
  int16_t error = scaled_reference - measured_value;

  pi_container.error_sum += error;

  //ive never seen such raw power before
  int16_t raw_power = pi_container.Kp*error + (int16_t)(pi_container.sample_time*(pi_container.Ki*pi_container.error_sum));

  //return kp*e + T*ki*int(e)
  pi_container.current_power = 0.01*(raw_power);
  pi_container.last_encoder_value = measured_value;
}

int16_t pos_controller_get_power() {
  return pi_container.current_power;
  //return 0;
}
