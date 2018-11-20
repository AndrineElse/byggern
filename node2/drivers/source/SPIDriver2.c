#define F_CPU 16000000

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "../include/SPIDriver2.h"

void SPI_init(void) {
    /* Set MOSI and SCK output, all others input */
  DDRB = (1<<DDB7)|(1<<DDB2)|(1<<DDB1)|(1<<DDB0);
  /* Enable SPI, Master, set clock rate fck/16 */
  SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);

}

void SPI_write(char cData) {
    /* Start transmission */

  SPDR = cData;
  /* Wait for transmission complete */

  while(!(SPSR & (1<<SPIF)));

}

char SPI_read() {
  SPI_write(0x00);
  while(!(SPSR & (1<<SPIF)));

  return SPDR;
}
