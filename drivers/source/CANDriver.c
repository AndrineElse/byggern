#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "../include/MCP2515.h"
#include "../include/MCP2515Driver.h"
#include "../include/CANDriver.h"

void CAN_init(){
  mcp2515_write(MCP_CANCTRL, (MODE_LOOPBACK)|(ABORT_TX));
}


void send_CAN_msg(struct CAN_msg* msg){
  mcp2515_request_to_send();
  mcp2515_write(MCP_TXB0SIDL,((msg->id)&(0x07)<<5));
  mcp2515_write(MCP_TXB0SIDH,(msg->id)>>3);

  //NB! need to finish the write with adress for data
  for(int i = 0; i < msg->length; i ++){
    mcp2515_write(msg->data[i]);
  }
}


struct CAN_msg* receive_msg(){
  //TO DO

}
