//system clock frequency, used by util/delay, 16MHz for node 2, 5MHz for node 1
#define F_CPU 16000000

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "../include/SPIDriver2.h"
#include "../include/MCP25152.h"
#include "../include/MCP2515Driver2.h"

void mcp2515_reset(){
  PORTB &= ~(1<<DDB7);
  SPI_write(MCP_RESET);

  PORTB |= (1<<DDB7);
}

uint8_t mcp2515_read(uint8_t address) {

  uint8_t result;
  PORTB &= ~(1<<DDB7); // Select CAN-controller

  SPI_write(MCP_READ); // Send read instruction

  SPI_write(address); // Send address

  result = SPI_read(); // Read result
  PORTB |= (1<<DDB7); // Deselect CAN-controller

  return result;
}

void mcp2515_read_store_pointer(uint8_t address, uint8_t* value){
  uint8_t result;
  PORTB &= ~(1<<DDB7); // Select CAN-controller
  SPI_write(MCP_READ); // Send read instruction

  SPI_write(address); // Send address

  result = SPI_read(); // Read result
  PORTB |= (1<<DDB7); // Deselect CAN-controller

  *value =  result;
}


uint8_t mcp2515_init() {
  uint8_t value;
  SPI_init(); // Initialize SPI
  mcp2515_reset(); // Send reset-command

  // Self-test
  mcp2515_read_store_pointer(MCP_CANSTAT, &value);

  if ((value & MODE_MASK) != MODE_CONFIG) {
    printf("MCP2515 is NOT in configuration mode after reset!\n");
    return 1;
  }
  // More initialization
  // set mode to normal?


  return 0;
}

uint8_t mcp2515_read_status(){
  PORTB &= ~(1<<DDB7);
  // SPI_write(MCP_CANSTAT);
  uint8_t read_value = mcp2515_read(MCP_CANSTAT);
  PORTB |= (1<<DDB7);
  return read_value;
}

void mcp2515_request_to_send(){
  PORTB &= ~(1<<DDB7);
  SPI_write(0x81);
  PORTB |= (1<<DDB7); //may be high???
}

void mcp2515_bit_modify(uint8_t address, uint8_t mask, uint8_t data){
  PORTB &= ~(1<<DDB7);
  SPI_write(MCP_BITMOD);
  SPI_write(address);
  SPI_write(mask);
  SPI_write(data);
  PORTB |= (1<<DDB7);
}

void mcp2515_write(uint8_t address, uint8_t data){
  PORTB &= ~(1<<DDB7);
  SPI_write(MCP_WRITE);
  SPI_write(address);
  SPI_write(data);
  PORTB |= (1<<DDB7);
}
