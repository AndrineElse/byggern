// WIll use UART to send raw data to the web server



#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "../include/UARTDriver2.h"

// Send 0x10 Score Position
// 0x12
void USART_Transmit_STXETX(uint16_t pay, uint8_t id_byte){
  cli();
  switch(id_byte){
    case 0x10:  // Score message
    //dosomething
      uint8_t length = 5;
      uint8_t payload[length];
      payload[0] = 0x10;
      payload[1] = 0
      payload[2] = 0
      payload[3] = pay;
      payload[4] = pay & 0xFF;
      // return 0x11;
      break;

    case 0x12:  // Position
    //dosomething
      uint8_t length = 3;
      uint8_t payload[length];
      payload[0] = 0x12;
      payload[1] = pay;
      payload[2] = pay & 0xFF;
      // return payload
      break;

    case 0x14:  // Lives
    //dosomething
      uint8_t length = 5;
      uint8_t payload[length];
      payload[0] = 0x14;
      payload[1] = 0;
      payload[2] = 0;
      payload[3] = 0;
      payload[4] = pay & 0xFF;
      // return 0x15;
      break;
    //add more cases here

    default:
      printf("Wrong id byte\n\r");
      break;
    }
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

  USART_Transmit(2); // STX
	USART_Transmit(0); // LEN0
	USART_Transmit(length); // LEN1
  for (int i = 0; i < length; i++){
    USART_Transmit(payload[i]);
  }

  //msg = STX 0x02 + LEN[0] uint8_t + LEN[1] uint8_t + Payload[0..LEN] + ETX 0x03;
  /* Put data into buffer, sends the data */
  USART_Transmit(0x03); // ETX
  sei();
}




uint8_t* USART_Receive_STXETX(){


  //uint8_t data[length];
  // VI henter ut av register, 2, len, len, payload, 3
  cli();
  uint8_t value = USART_Receive();
  uint16_t len;
  // uint8_t payload;


  //if (value == 2) {
    // value = USART_Receive();
    uint8_t length_0 = USART_Receive();
    uint8_t length_1 = USART_Receive();
    len = length_1;
    //len |= (length_0 << 1);
    printf("LEN: %x\n\r", len );
    
    uint8_t id_byte = = USART_Receive();
    
    switch(id_byte){
    case 0x11:  // Request new game
    //dosomething
      uint8_t payload[1];
      payload[0] = 0x11;
      // return 0x11;
      break;

    case 0x13:  // Request position
    //dosomething
      uint8_t payload[2];
      payload[0] = USART_Receive();
      payload[1] = USART_Receive();
      // return payload
      break;

    case 0x15:  // Fire
    //dosomething
      uint8_t payload[1];
      payload[0] = 0x15;
      // return 0x15;
      break;
    //add more cases here

    default:
      printf("Wrong id byte\n\r");
      break;
    }
    /*
    for (uint8_t i = 0; i < len; i++){
      payload[i] = USART_Receive();
    }*/
    printf("First byte: %x\n\r", payload[0]);
    printf("Second byte: %x\n\r", payload[1]);
    printf("Third byte: %x\n\r", payload[2]);
    //printf("Fourth byte: %x\n\r", payload[3]);
    //printf("Fifth byte: %x\n\r", payload[4]);
    while(USART_Receive() != 3){}
    //value = USART_Receive();
    sei();
    //if (value == 3) {
    return payload;
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
