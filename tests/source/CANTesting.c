#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "../../drivers/include/MCP2515.h"
#include "../../drivers/include/MCP2515Driver.h"
#include "../../drivers/include/CANDriver.h"
#include "../include/CANTesting.h"

void testCAN() {
  struct CAN_msg msg;
  msg.id = 1;
  uint8_t array[8] = {1,2,3,4,5,6,7,8};
  for (int j = 0; j < 8; j++){
    msg.data[j] = array[j];
  }

  msg.length = 8;

  send_CAN_msg(&msg);
  _delay_ms(100);
  struct CAN_msg recieved_message = receive_msg();
  // recieved_message = receive_msg();

  printf("Message ID: %d \n\r",recieved_message.id);
  for (int i = 0; i < recieved_message.length ; i++){
    printf("Message data: %d , i = %d \n\r",recieved_message.data[i], i);
  }
  printf("Message length : %d \n\r",recieved_message.length);
}
