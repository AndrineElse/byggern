// WIll use UART to send raw data to the web server


#include "../include/UARTDriver2.h"
#include <avr/io.h>
#include <stdio.h>


void USART_Init( unsigned int  ubrr )
{
  /* Set baud rate */
  UBRR0H = (unsigned char)(ubrr>>8);
  UBRR0L = (unsigned char)ubrr;
  /* Enable receiver and transmitter */
  UCSR0B = (1<<RXEN0)|(1<<TXEN0);
  /* Set frame format: 8data, 2stop bit */
  UCSR0C = (1<<USBS0)|(1<<UCSZ00)|(1<<UCSZ01); // not sure whether to use ucsz00, 01 or 02...
}


void USART_Transmit( unsigned char data )
{
  /* Wait for empty transmit buffer */
  while ( !( UCSR0A & (1<<UDRE0)) );
  /* Put data into buffer, sends the data */
  UDR0 = data;
}

unsigned char USART_Receive( void )
{
  /* Wait for data to be received */
  while ( !(UCSR0A & (1<<RXC0)) );
  /* Get and return received data from buffer */
  return UDR0;
}

// Send 0x10 Score Position
// 0x12
void USART_Transmit_STXETX( uint32_t payload, uint8_t firstByte )
{
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
}

uint16_t USART_Receive_STXETX( void )
{
  uint8_t value = USART_Receive( void );
  if (value == 0x02){
    uint8_t len_0 = USART_Receive( void );
    uint8_t len_1 = USART_Receive( void );
    uint16_t payload = USART_Receive( void );
  }

  return payload;
}
