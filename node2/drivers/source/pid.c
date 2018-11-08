#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "../include/pid.h"
#include "../include/motorDriver.h"
#include "../include/servoDriver.h"

// Encoder -1207 til 7480-7536 ish = ca 8700
// Joystick Y -100 to 96 = 196

void pid_init(double p_factor, double i_factor, struct PID_data *pid){
  pid->error_sum = 0;
  pid->Kp = p_factor;
  pid->Ki = i_factor;
  pid->position = -4000;
}

int16_t pid_controller(struct PID_data *pid){
  JoystickCoords coords = get_new_joystick_values();
  uint16_t actual_encoder_value = read_motor_encoder();
  int16_t target_value = coords.y*40;

  pid->position += actual_encoder_value;
  int16_t error = target_value - pid->position;
  uint8_t T = 0.1;
  printf("Error: %d\n\r", error);
  printf("Joystick Y: %d\n\r", target_value);
  pid->error_sum += error;
  int16_t u = (pid->Kp)*error + T*(pid->Ki)*(pid->error_sum);

  return u;
}

// - 100 : -1207
// 0 : 3231,77
// 96 = 7500
// 44.38 - 100
