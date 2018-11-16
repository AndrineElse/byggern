#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "../include/UARTDriver2.h"

void USART_Transmit_STXETX(uint16_t pay, uint8_t id_byte){
  printf("TRANSMIT:\n\r" );
  cli();
  USART_Transmit(0x02); // STX
	USART_Transmit(0x00); // LEN0

  uint8_t length;
    if(id_byte == 0x10){ // Score message

      length = 5;
      USART_Transmit(length); // LEN1
      uint8_t payload_10[length];
      payload_10[0] = 0x10;
      payload_10[1] = 0;
      payload_10[2] = 0;
      payload_10[3] = (pay>>8)&0xFF;
      payload_10[4] = pay & 0xFF;
      for (int i = 0; i < length; i++){
        USART_Transmit(payload_10[i]);
      }
      USART_Transmit(0x03); // ETX
    }

    else if(id_byte == 0x12){ //Position
      length = 0x03;
      USART_Transmit(length); // LEN1
      //uint8_t payload[length];
      uint8_t payload_12[length];
      payload_12[0] = 0x12;
      payload_12[1] = (pay>>8)&0xFF;
      payload_12[2] = pay & 0xFF;
      for (int i = 0; i < length; i++){
        USART_Transmit(payload_12[i]);
      }
      USART_Transmit(0x03); // ETX
    }

    else if(id_byte == 0x14){ //Lives
      length = 5;
      USART_Transmit(length); // LEN1

      uint8_t payload_14[length];
      payload_14[0] = 0x14;
      payload_14[1] = 0x00;
      payload_14[2] = 0x00;
      payload_14[3] = (pay>>8)&0xFF;
      payload_14[4] = pay & 0xFF;
      for (int i = 0; i < length; i++){
        USART_Transmit(payload_14[i]);
      }/*
      USART_Transmit(payload_14[0]);
      USART_Transmit(payload_14[1]);
      USART_Transmit(payload_14[2]);
      USART_Transmit(payload_14[3]);
      USART_Transmit(payload_14[4]);*/
      USART_Transmit(0x03); // ETX
      /*
      printf("Payload: %x\n\r", payload_14[0]);
      printf("Payload: %x\n\r", payload_14[1]);
      printf("Payload: %x\n\r", payload_14[2]);
      printf("Payload: %x\n\r", payload_14[3]);
      printf("Payload: %x\n\r", payload_14[4]);*/
      // return 0x15;
      //break;
    //add more cases here
  }

    /*default:
      printf("Wrong id byte\n\r");
      break;
    }*/
  /*
  uint8_t length = 5;
  uint8_t data[length];
  data[0] = firstByte;
  data[1] = (payload >> 24);
  data[2] = (payload >> 16);
  data[3] = (payload >> 8);
  data[4] = payload & 0xFF;
  printf("data4: %x\n", data[4]);*/
  // length = number of bytes in payload
  // length |= (len_0 << 8);

  //USART_Transmit(2); // STX
	//USART_Transmit(0); // LEN0
	//USART_Transmit(length); // LEN1
  /*for (int i = 0; i < length; i++){
    USART_Transmit(payload[i]);
  }*/

  //msg = STX 0x02 + LEN[0] uint8_t + LEN[1] uint8_t + Payload[0..LEN] + ETX 0x03;
  /* Put data into buffer, sends the data */
  sei();
}




uint8_t* USART_Receive_STXETX(){
  printf("RECEIVE:\n\r" );

  //uint8_t data[length];
  // VI henter ut av register, 2, len, len, payload, 3
  cli();
  //uint8_t value = USART_Receive();
  //printf("value: %x\n\r", value);
  while(USART_Receive() != 0x02){
  };
  uint8_t len;

  uint8_t length_0 = USART_Receive();
  uint8_t length_1 = USART_Receive();
  len = length_1;
  //len |= (length_0 << 1);
  printf("LEN: %x\n\r", len );

  uint8_t id_byte = USART_Receive();
  printf("id: %x\n\r", id_byte );
  // uint8_t payload[len];
    //switch(id_byte){
    //case 0x11:  // Request new game
    if(id_byte == 0x11){
    //dosomething
      //uint8_t payload[1];
      len = 1;
      uint8_t payload_11[len];
      payload_11[0] = 0x11;
      printf("First byte: %x\n\r", payload_11[0]);

      return payload_11;
      // return 0x11;
    }

    if(id_byte == 0x13){
    //case 0x13:  // Request position
    //dosomething
      //uint8_t payload[2];
      len = 2;
      uint8_t payload_13[len];
      payload_13[0] = USART_Receive();
      payload_13[1] = USART_Receive();
      printf("First byte: %x\n\r", payload_13[0]);
      printf("Second byte: %x\n\r", payload_13[1]);

      return payload_13;
      // return payload
      //break;
    }

    if(id_byte == 0x15){
    //case 0x15:  // Fire
    //dosomething
      //uint8_t payload[1];
      len = 1;
      uint8_t payload_15[len];
      payload_15[0] = 0x15;
      printf("First byte: %x\n\r", payload_15[0]);

      return payload_15;
      // return 0x15;
      //break;
    //add more cases here

    /*default:
      printf("Wrong id byte\n\r");
      //break;*/
    }
  uint8_t value = USART_Receive();
  printf("end value: %x\n\r", value);
  //value = USART_Receive();
  sei();
  //if (value == 3) {
  return 0;
    /*
    for (uint8_t i = 0; i < len; i++){
      payload[i] = USART_Receive();
    }*/
  /*printf("First byte: %x\n\r", payload[0]);
  printf("Second byte: %x\n\r", payload[1]);
  printf("Third byte: %x\n\r", payload[2]);
  //printf("Fourth byte: %x\n\r", payload[3]);
  //printf("Fifth byte: %x\n\r", payload[4]);
  // while(USART_Receive() != 3){}
  /*if (USART_Receive() == 3){
    return payload;
  }*/

    //}
  //}
  //if (value == 0x02){
    //uint8_t len_0 = USART_Receive();
    //uint8_t len_1 = USART_Receive();
    //uint8_t byte = USART_Receive();
    //if (firstByte == 0x13){
      //length = 2;




    //}
    // uint8_t payload = USART_Receive();

    //uint8_t last = USART_Receive();
    // return 0;
  //}
}
