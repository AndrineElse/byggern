#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <float.h>
#include <avr/interrupt.h>
#include "../include/PIDriver.h"
#include "../include/motorDriver.h"
#include "../include/servoDriver.h"

// Encoder -1207 til 7480-7536 ish = ca 8700
// Joystick Y -100 to 96 = 196

void pid_init(int8_t p_factor, int8_t i_factor, struct PID_data *pid){
  pid->error_sum = 0;
  pid->Kp = p_factor;
  pid->Ki = i_factor;
  pid->position = 0;
}

int16_t pid_controller(struct PID_data *pid){
  JoystickCoords coords = input_container_get_ptr()->joystick;
  uint16_t actual_encoder_value = -read_motor_encoder();
  int16_t target_value = coords.y;

  pid->position += actual_encoder_value;
  int16_t error = target_value - pid->position/40;
  float T = 0.1;
  printf("Joystick Y: %d\n\r", target_value);
  printf("Position: %d\n\r", pid->position/40);
  printf("Error: %d\n\r", error);
  printf("Kp %d\n\r", pid->Kp);

  pid->error_sum += error;
  printf("ErrorSum: %d\n\r", pid->error_sum);
  int16_t u = (pid->Kp)*error + T*(pid->Ki)*(pid->error_sum);
  if (u > 127){
    return 127;
  }
  else if (u < -127){
    return -127;
  }
  return u;
}

// - 100 : -1207
// 0 : 3231,77
// 96 = 7500
// 44.38 - 100
