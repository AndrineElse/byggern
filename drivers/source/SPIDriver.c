#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "../include/SPIDriver.h"

void SPI_init(void) {
    /* Set MOSI and SCK output, all others input */
  DDRB |= (1<<DDB4)|(1<<DDB5)|(1<<DDB7);
    /* Enable SPI, Master, set clock rate fck/16 */
  SPCR |= (1<<MSTR)|(1<<SPR0);
  SPCR |= (1<<SPE);
  PORTB |= 0x10;
}

void SPI_write(char cData) {
    /* Start transmission */
  //PORTB &= ~0x10;
  SPDR = cData;
    /* Wait for transmission complete */
  SPSR = (1<<SPIF);
  while(!(SPSR & (1<<SPIF)));
  //PORTB |= 0x010;
}

char SPI_read() {
  SPI_write(0x00);
  while(!(SPSR & (1<<SPIF)));

  return SPDR;
}
