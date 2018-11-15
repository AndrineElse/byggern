// WIll use UART to send raw data to the web server



#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "../include/UARTDriver2.h"

// Send 0x10 Score Position
// 0x12
void USART_Transmit_STXETX(uint32_t payload, uint8_t firstByte){
  cli();
  uint8_t length = 5;
  uint8_t data[length];
  data[0] = firstByte;
  data[1] = (payload >> 24);
  data[2] = (payload >> 16);
  data[3] = (payload >> 8);
  data[4] = payload & 0xFF;
  // length = number of bytes in payload
  // length |= (len_0 << 8);

  USART_Transmit(0x02); // STX
	USART_Transmit(0x00); // LEN0
	USART_Transmit(length); // LEN1
  for (int i = 0; i < length; i++){
    USART_Transmit(data[i]);
  }

  // msg = STX 0x02 + LEN[0] uint8_t + LEN[1] uint8_t + Payload[0..LEN] + ETX 0x03;
  /* Put data into buffer, sends the data */
  USART_Transmit(0x03); // ETX
  sei();
}

uint16_t USART_Receive_STXETX( uint8_t firstByte )
{
  uint8_t length = 2;
  uint8_t data[length];
  cli();
  uint8_t first = USART_Receive();
  //if (value == 0x02){
    uint8_t len_0 = USART_Receive();
    uint8_t len_1 = USART_Receive();
    uint8_t byte = USART_Receive();
    if (firstByte == 0x13){
      length = 2;
      data[0] = USART_Receive();
      data[1] = USART_Receive();


    }
    // uint8_t payload = USART_Receive();

    uint8_t last = USART_Receive();
    sei();
    return first;
  //}
}
