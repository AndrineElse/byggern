#define F_CPU 5000000
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "../include/MCP2515.h"
#include "../include/MCP2515Driver.h"
#include "../include/CANDriver.h"
#include "../../containers/include/gameStatusContainer.h"

void CAN_init(){
  mcp2515_init();

  mcp2515_bit_modify(MCP_RXB0CTRL, 0x60 , 0xFF);
  // mcp2515_bit_modify(MCP_CANINTE, 0x40 , 0xFF);
  mcp2515_bit_modify(MCP_CANINTE, 0x01 , 0xFF);
  //mcp2515_bit_modify(MCP_CANCTRL, 0xE0, MODE_LOOPBACK);
  mcp2515_bit_modify(MCP_CANCTRL, 0xE0, MODE_NORMAL);

}

void CAN_init_interrupt() {
  //enable int1 (PD3) to trigger on new can message ready

  //  enable int1 flag
  GICR |= 0x80;

  //  set int2 to trigger on low value
  //donothing, is trigger on low value by default

  //  set PD3 as input
  //donothing, should be input by default
}

void send_CAN_msg(struct CAN_msg* msg){
  cli();
  // mcp2515_write(MCP_TXB0SIDL,((msg->id)&(0x07)<<5));
  mcp2515_bit_modify(MCP_TXB0SIDL, 0xD0, (((msg->id)&(0x07))<<5));
  mcp2515_write(MCP_TXB0SIDH,(msg->id)>>3);
  for(int i = 0; i < msg->length; i ++){
    mcp2515_write(MCP_TXB0D0+i,msg->data[i]);
  }
  mcp2515_write(MCP_TXB0DLC,msg->length);

  mcp2515_request_to_send();

  sei();
}


struct CAN_msg receive_msg(){
  struct CAN_msg msg;
  uint8_t lowerID;
  uint8_t upperID;


  mcp2515_read_store_pointer(MCP_RXB0SIDL,&lowerID);
  mcp2515_read_store_pointer(MCP_RXB0SIDH,&upperID);

  msg.id = (lowerID>>5);
  msg.id |= (upperID<<3);
  msg.length = mcp2515_read(MCP_RXB0DLC)&0xF;

  //printf("length: %d\n\r", msg.length);

  for(int i = 0; i < msg.length; i ++){
    mcp2515_read_store_pointer(MCP_RXB0D0+i,msg.data + i);
    //printf("data : %d\n\r", msg.data[i]);
  }
  //clear interrupt flag after reading message
  mcp2515_bit_modify(MCP_CANINTF,0x01,0);

  return msg;
}

ISR(INT1_vect){
  cli();
  CAN_message_handler();
  sei();
}

void CAN_message_handler() {
  struct CAN_msg new_message = receive_msg();
  if(new_message.id ==2){
    game_status_container_update(new_message);
  }
}
