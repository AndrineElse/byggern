#define F_CPU 16000000

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "../include/MCP25152.h"
#include "../include/MCP2515Driver2.h"
#include "../include/CANDriver2.h"
#include "../../containers/include/userInputContainer.h"
#include "../../containers/include/gameDataContainer.h"

void CAN_init(){
  mcp2515_init();
  // TODO not sure what this is
  mcp2515_bit_modify(MCP_RXB0CTRL, 0x60 , 0xFF);

  // TODO not sure what this is
  // mcp2515_bit_modify(MCP_CANINTE, 0x40 , 0xFF);

  // enables interrupt on message transfer to RX0
  mcp2515_bit_modify(MCP_CANINTE, 0x01 , 0xFF);

  // sets the controller to normal mode
  mcp2515_bit_modify(MCP_CANCTRL, 0xE0, MODE_NORMAL);

}

void CAN_init_interrupt() {
  cli();
  // interrupt config for ordinary external interrupt on PD2.

  // enable int2
  EIMSK |= 0x04;

  // set int 2 trigger mode
  // donothing, trigger mode is already low level.

  // Set PD4 as input
  // donothing, should be input by default



  // interrupt config for PinChange interrupt on PB4
  /* NOT IN USE --------------------------------------------
  // enables external pin change interrupt 0
  PCICR |= 0x01;

  // sets portb pin 4 (PB4) as a pin change interrupt source
  PCMSK0 |= (1 << DDB4);

  // configure PB4 as an input
  //donothing, should be input by default
  */ //NOT IN USE END --------------------------------------

  sei();
}


void send_CAN_msg(struct CAN_msg* msg){
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

  if (buffermsg.id == msg->id && buffermsg.data[0] == msg->data[0] && buffermsg.length == msg->length){
      mcp2515_request_to_send();
      _delay_ms(100);
  }
  //sjekk at request to send flag er høyt ???
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
  mcp2515_bit_modify(MCP_CANINTF,MCP_RX0IF,0);

  return msg;
}

ISR(INT2_vect) {
  cli();
  CAN_message_handler();
  sei();
}

// This function should be called whenever a interrupt
// corresponding to a new message being ready.
// It will load the message,
//    look at the id,
//    and perform the appropriate action
void CAN_message_handler(){
  struct CAN_msg new_message = receive_msg();
  switch(new_message.id){
    case 1:
      input_container_update(new_message);
      break;
    case 3:
      game_data_container_update(new_message);
      //printf("Recieved a game status message, num fails = %d \n", new_message.data[1]);
    break;
    //add more cases here

    default:
      printf("Message with unmapped ID loaded :(\n\r");
      break;
  }
}
