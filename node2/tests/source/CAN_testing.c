#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "../../drivers/include/MCP25152.h"
#include "../../drivers/include/MCP2515_driver.h"
#include "../../drivers/include/CAN_driver.h"
#include "../include/CAN_testing.h"

// MAPPING:
//joystickCoords id = 1

void CAN_test() {
  printf("testCan\n\r");
  struct CAN_msg recieved_message = CAN_receive_msg();
  printf("Message ID: %d \n\r",recieved_message.id);

  if (recieved_message.id == 1){
    for (int i = 0; i < recieved_message.length ; i++){
      int8_t temp_data = (int8_t)recieved_message.data[i];
      printf("Message data: %d , i = %d \n\r",temp_data, i);
    }
  }

  printf("Message length : %d \n\r",recieved_message.length);
  _delay_ms(2000);
}
