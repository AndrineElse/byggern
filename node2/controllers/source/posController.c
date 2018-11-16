#define F_CPU 16000000

#include <float.h>
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include "../include/posController.h"

// Encoder -1207 til 7480-7536 ish = ca 8700
// Joystick Y -100 to 96 = 196

volatile struct PID_data pi_container;


void pos_controller_init(int8_t p_factor, int8_t i_factor, float sample_time, uint8_t encoder_factor) {
  //constants
  pi_container.Kp = p_factor;
  pi_container.Ki = i_factor;
  pi_container.sample_time = sample_time;
  pi_container.encoder_factor = encoder_factor;

  //variables
  pi_container.position = -4000;
  pi_container.error_sum = 0;
  pi_container.current_power = 0;
}


// Calculates a new power for the motor,
// should be in the range of -300, 300,
// motor saturates at -255, 255
// Params:
//    reference_value:
//      value from -100 to 100
void pos_controller_calculate_power(int8_t reference_value, int16_t measured_value) {

  pi_container.position += measured_value;
  int16_t error = reference_value - (int16_t)(pi_container.position/pi_container.encoder_factor);
  pi_container.error_sum += (error < 20 ? error : 0);

  //return kp*e + T*ki*int(e)
  pi_container.current_power = pi_container.Kp*error + (pi_container.sample_time*pi_container.Ki*pi_container.error_sum);
  printf("r: %d\n\r", reference_value);
  printf("x: %d\n\r", reference_value);
  printf("e: %d\n\r", error);
  printf("u: %d\n\r", pi_container.current_power);
}

int16_t pos_controller_get_power() {
  return pi_container.current_power;
  //return 0;
}
