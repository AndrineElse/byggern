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
#include "../include/servoDriver.h"
#include "../include/solenoidDriver.h"
#include "../../containers/include/userInputContainer.h"
#include "../../game/include/game.h"
#include "../../controllers/include/posController.h"
#include "../../game/include/gamePlayback.h"

void CAN_init(){
  mcp2515_init();
  mcp2515_bit_modify(MCP_RXB0CTRL, 0x60 , 0xFF);

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

  sei();
}


void send_CAN_msg(struct CAN_msg* msg){
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

  for(int i = 0; i < buffermsg.length; i ++){
    mcp2515_read_store_pointer(MCP_TXB0D0+i,buffermsg.data + i);
  }

  if (buffermsg.id == msg->id && buffermsg.data[0] == msg->data[0] && buffermsg.length == msg->length){
      mcp2515_request_to_send();
      _delay_ms(100);
  }
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



  for(int i = 0; i < msg.length; i ++){
    mcp2515_read_store_pointer(MCP_RXB0D0+i,msg.data + i);

  }
  mcp2515_bit_modify(MCP_CANINTF,MCP_RX0IF,0);

  return msg;
}

ISR(INT2_vect) {
  CAN_message_handler();
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
      if (game_get_playing_status()) {
        servo_update_position(input_container_get_ptr()->joystick.x);
        solenoid_update_status(input_container_get_ptr()->joystickButton);
        playback_set_next_sample(pos_controller_get_power(),input_container_get_ptr()->joystick.x,input_container_get_ptr()->joystickButton);
      }
      break;
    case 4:
      game_select_controller(new_message);
      break;

    default:
      break;
  }
}
