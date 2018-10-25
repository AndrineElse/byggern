#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "../include/MCP2515.h"
#include "../include/MCP2515Driver.h"
#include "../include/CANDriver.h"

void CAN_init(){
  mcp2515_init();

  mcp2515_bit_modify(MCP_RXB0CTRL, 0x60 , 0xFF);
  // mcp2515_bit_modify(MCP_CANINTE, 0x40 , 0xFF);
  mcp2515_bit_modify(MCP_CANINTE, 0x01 , 0xFF);
  //mcp2515_bit_modify(MCP_CANCTRL, 0xE0, MODE_LOOPBACK);
  mcp2515_bit_modify(MCP_CANCTRL, 0xE0, MODE_NORMAL);

}


void send_CAN_msg(struct CAN_msg* msg){
  /*
  printf("Mode : %02x\n\r", mcp2515_read(MCP_CANCTRL));
  printf("CanStat : %02x\n\r", mcp2515_read(MCP_CANSTAT));
  printf("Recievebuffer: %d", mcp2515_read(MCP_RXB0D0));
  printf("Lengthbuffer: %d\n\r",mcp2515_read(MCP_RXB0DLC)&0x7);
*/


  // mcp2515_write(MCP_TXB0SIDL,((msg->id)&(0x07)<<5));
  mcp2515_bit_modify(MCP_TXB0SIDL, 0xD0, (((msg->id)&(0x07))<<5));
  mcp2515_write(MCP_TXB0SIDH,(msg->id)>>3);
  for(int i = 0; i < msg->length; i ++){
    mcp2515_write(MCP_TXB0D0+i,msg->data[i]);
  }
  mcp2515_write(MCP_TXB0DLC,msg->length);

  //see if buffermsg is equal to message being sent
  struct CAN_msg buffermsg;

  uint8_t lowerID;
  uint8_t upperID;

  mcp2515_read_store_pointer(MCP_TXB0SIDL,&lowerID);
  mcp2515_read_store_pointer(MCP_TXB0SIDH,&upperID);

  buffermsg.id = (lowerID>>5);
  buffermsg.id |= (upperID<<3);
  buffermsg.length = mcp2515_read(MCP_TXB0DLC)&0xF;

  /*mcp2515_read_store_pointe(MCP_TXB0SIDL,((buffermsg.id)&(0x07)<<5));
  mcp2515_read_store_pointer(MCP_TXB0SIDH,(buffermsg.id)>>3);
  buffermsg.length = mcp2515_read(MCP_TXB0DLC); // data length command, RXB0DLC*/

  for(int i = 0; i < buffermsg.length; i ++){
    mcp2515_read_store_pointer(MCP_TXB0D0+i,buffermsg.data + i);
  }
/*
  printf("readlength: %d\n\r", buffermsg.length);
  printf("setlength: %d\n\r", msg->length);

  printf("readID: %d\n\r", buffermsg.id);
  printf("setID: %d\n\r", msg->id);
*/
  for (int i = 0; i < msg->length;i++){
    /*
    printf("readData: %d\n\r", buffermsg.data[i]);
    printf("setData: %d\n\r", msg->data[i]);
    */
  }
  if (buffermsg.id == msg->id && buffermsg.data[0] == msg->data[0] && buffermsg.length == msg->length){
      //printf("buffer == message recieved\n\r");
      mcp2515_request_to_send();
      _delay_ms(100);
      //printf("TXReq %02x\n", mcp2515_read(0x30));
  }
  //sjekk at request to send flag er høyt ???
}


struct CAN_msg receive_msg(){
  printf("CANINTF %02x\n", mcp2515_read(MCP_CANINTF));
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

  mcp2515_bit_modify(MCP_CANINTF,0x01,MCP_RX0IF);

  return msg;

  // Mulig feil her!
  //Får kun samme mld flere ganger på rad! må finne en måte å overskrive gamle meldinger
}
