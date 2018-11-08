#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "../../drivers/include/MCP2515.h"
#include "../../drivers/include/MCP2515_driver.h"
#include "../../drivers/include/CAN_driver.h"
#include "../include/CAN_testing.h"

void CAN_test() {
  struct CAN_msg msg;
  msg.id = 1;
  uint8_t array[8] = {1,1,1,1,1,1,1,1};
  for (int j = 0; j < 8; j++){
    msg.data[j] = array[j];
  }

  msg.length = 8;

  CAN_send_msg(&msg);
  /*_delay_ms(100);
  struct CAN_msg recieved_message = CAN_receive_msg();
  // recieved_message = CAN_receive_msg();

  printf("Message ID: %d \n\r",recieved_message.id);
  for (int i = 0; i < recieved_message.length ; i++){
    printf("Message data: %d , i = %d \n\r",recieved_message.data[i], i);
  }
  printf("Message length : %d \n\r",recieved_message.length);*/
}
