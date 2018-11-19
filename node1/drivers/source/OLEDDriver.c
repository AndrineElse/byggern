#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "../../fonts.h"
#include "../include/OLEDDriver.h"
#include "../include/UARTdriver.h"

volatile char* command_address = (char*)0x1000;
volatile char* data_address = (char*)0x1200;

void OLED_write_command(uint8_t c) {
  command_address[0] = c;
}


void OLED_clear(){
  for (int i = 0; i < 8; i++){
    OLED_write_command(0xb0 + i);
    for (int j = 0; j < 128; j++){
      data_address[i] = 0x0;
    }
  }
}

void OLED_fill(){
  for (int i = 0; i < 8; i++){
    OLED_write_command(0xb0 + i);
    for (int j =0; j < 128; j++){
      data_address[i] = 0xFF;
    }
  }
}

void OLED_init(){
  cli();
  OLED_write_command(0xae);        //  display  off
  OLED_write_command(0xa1);        //segment  remap
  OLED_write_command(0xda);        //common  pads  hardware:  alternative
  OLED_write_command(0x12);
  OLED_write_command(0xc8);        //common  output scan direction:com63~com0
  OLED_write_command(0xa8);        //multiplex  ration  mode:63
  OLED_write_command(0x3f);
  OLED_write_command(0xd5);        //display  divide ratio/osc. freq. mode
  OLED_write_command(0x80);        //   -> Resets oscillator frequency
  OLED_write_command(0x81);        //contrast  control
  OLED_write_command(0x50);
  OLED_write_command(0xd9);        //set  pre-charge  period
  OLED_write_command(0x21);
  OLED_write_command(0x20);        //Set  Memory  Addressing  Mode
  OLED_write_command(0x02);        //   -> Page Addressing Mode
  OLED_write_command(0xdb);        //VCOM  deselect  level  mode
  OLED_write_command(0x30);
  OLED_write_command(0xad);        //master  configuration
  OLED_write_command(0x00);
  OLED_write_command(0xa4);        //out  follows  RAM  content
  OLED_write_command(0xa6);        //set  normal  display
  OLED_write_command(0xaf);        //  display  on
  sei();

}

void OLED_pos(uint8_t row, uint8_t column){
  cli();
  OLED_write_command(0xb0 + row); //rown (page select)
  OLED_write_command(column & 0xf);
  OLED_write_command((column >> 4) + 0x10);
  sei();
}

void OLED_write_data(char c){
  data_address[0] = c;
}

void OLED_write_char(uint8_t b){
  if(b < 32){
    return;
  }
  uint8_t font_table_index = b - 32;

  for (int i=0; i < 5; i++) {
    OLED_write_data(pgm_read_byte(&font5[font_table_index][i]));
  }
}



void OLED_buffer_print_line(char* string, uint8_t line, uint8_t inverseFlag){
  uint16_t i = 0;
  uint16_t currentByte = line*128;
  while(string[i]) {
    uint8_t currentChar = (uint8_t)string[i];
    if(currentChar < 32){
      continue;
    }
    uint8_t font_table_index = currentChar - 32;

    for (int j=0; j < 5; j++) {
      OLED_update_buffer_single_byte(currentByte, (inverseFlag ? ~pgm_read_byte(&font5[font_table_index][j]) : pgm_read_byte(&font5[font_table_index][j])));
      //printf("currentByte: %d, asciivalue: %d\n\r", currentByte, pgm_read_byte(&font5[font_table_index][j]));
      currentByte++;
    }
    i++;
  }
}


/*buffer for oled starts at 0x1800
* uses 1024 addresses, each containing 8 bits
* should have final adress at 0x1bFF
* each mem is stored sequentially
* first 128 is each byte of first row, etc..
*/
void OLED_update_buffer_single_byte(uint16_t address, uint8_t data ){
  char* ext_ram = (char*)0x1800;
  ext_ram[address] = data;
}

/*Similar to above, but write lots of bytes in sequence, useful for whole lines etc
*
*/
void OLED_update_buffer_array(uint16_t start_address, uint8_t* data, uint8_t data_amount) {
  if(start_address + data_amount > 1023){
    printf("attempted to update buffer outside of screen, implement wrap around?");
    return;
  }
  for (uint8_t i = 0; i < data_amount; i++){
    OLED_update_buffer_single_byte(start_address + i, data[i]);
  }
}

/*Similar to above, assumes data array has 127 elements.
* line should be some value between 0 and 7
*/
void OLED_update_buffer_line(uint8_t line, uint8_t* data) {
  if(line > 7){
    printf("attempted to update buffer with invalid line, implement wrap around?");
    return;
  }
  uint16_t address = ((uint16_t)line)*128;
  OLED_update_buffer_array(address,data,128);
}


/*mode should be a number between 0 and 2, key:
* 0: Horizontal mode
* 1: Vertical mode
* 2: Page mode
*/
void OLED_set_access_mode(uint8_t mode) {
  cli();
  if(mode < 3) {
    OLED_write_command(0x20);
    OLED_write_command(mode);
  }
  sei();
}

/* sets coordinate bounds for horizontal addressing mode
* bounds should be within [0,127]
*/
void OLED_set_horizontal_bounds(uint8_t lower, uint8_t upper) {

  OLED_write_command(0x21);
  OLED_write_command(lower);
  OLED_write_command(upper);

}

/* sets coordinate bounds for horizontal addressing mode
* bounds should be within [0,7]
*/
void OLED_set_vertical_bounds(uint8_t lower, uint8_t upper) {

  OLED_write_command(0x22);
  OLED_write_command(lower);
  OLED_write_command(upper);
}

void OLED_init_buffer_mode(){
  cli();
  OLED_set_access_mode(0);
  sei();
}

void OLED_buffer_update_screen(){
  cli();
  OLED_set_horizontal_bounds(0,127);
  OLED_set_vertical_bounds(0,7);
  char* ext_ram = (char*)0x1800;
  for (uint16_t i = 0; i < 1024; i++){
    //printf("currentByte: %d, asciivalue: %d\n\r", i, ext_ram[i]);
    OLED_write_data(ext_ram[i]);
  }
  sei();
}

void OLED_buffer_clear(){
  cli();
  for(uint16_t i = 0; i < 1024; i++ ) {
    OLED_update_buffer_single_byte(i, 0x00);
  }
  sei();
}

void OLED_buffer_fill(){
  for(uint16_t i = 0; i < 1024; i++ ) {
    OLED_update_buffer_single_byte(i, 0xFF);
  }
}
//drawingfunctions


void print_highscore_node(uint8_t place, uint8_t username, uint8_t score_H, uint8_t score_L){
  uint16_t currentByte = (place+1)*128; // select line

  char* number;
  switch(place){
    case 1:
      number = "1. ";
      break;
    case 2:
      number = "2. ";
      break;
    case 3:
      number = "3. ";
      break;
  }
  uint16_t a = 0;
  while(number[a]) {
    uint8_t currentChar = (uint8_t)number[a];
    if(currentChar < 32){
      continue;
    }
    uint8_t font_table_index = currentChar - 32;
  for (int j=0; j < 5; j++) {
      OLED_update_buffer_single_byte(currentByte, (pgm_read_byte(&font5[font_table_index][j])));
      //printf("currentByte: %d, asciivalue: %d\n\r", currentByte, pgm_read_byte(&font5[font_table_index][j]));
      currentByte++;
    }
  }

  // PLACE
  /*char* number;
  switch(place){
    case 1:
      number = "1. ";
      break;
    case 2:
      number = "2. ";
      break;
    case 3:
      number = "3. ";
      break;
  }
  uint16_t a = 0;
  while(number[a]) {
    uint8_t currentChar = (uint8_t)number[a];
    if(currentChar < 32){
      continue;
    }
    uint8_t font_table_index = currentChar - 32;
  for (int j=0; j < 3; j++) {
      OLED_update_buffer_single_byte(currentByte, (pgm_read_byte(&font5[font_table_index][j])));
      //printf("currentByte: %d, asciivalue: %d\n\r", currentByte, pgm_read_byte(&font5[font_table_index][j]));
      currentByte++;
    }
  }*/
/*
  // USERNAME
  char* name;
  uint8_t length;
  switch(username){
    case 0:
      name = "Magne";
      length = 5;
      break;
    case 1:
      name = "Andrine";
      length = 7;
      break;
    case 2:
      name = "Thea";
      length = 4;
      break;
  }
  //OLED_buffer_print_line(name, 2, 0);

  uint16_t b = 0;
  while(name[b]) {
    uint8_t currentChar = (uint8_t)name[b];
    if(currentChar < 32){
      continue;
    }
    uint8_t font_table_index = currentChar - 32;
  for (int j=0; j < 5; j++) {
      OLED_update_buffer_single_byte(currentByte, (pgm_read_byte(&font5[font_table_index][j])));
      //printf("currentByte: %d, asciivalue: %d\n\r", currentByte, pgm_read_byte(&font5[font_table_index][j]));
      currentByte++;
    }
  }
/*
  char* score = (char*)0;//(score_H << 8) + score_L;
  uint16_t c = 0;
  while(score[c]) {
    uint8_t currentChar = (uint8_t)score[c];
    if(currentChar < 32){
      continue;
    }
    uint8_t font_table_index = currentChar - 32;
  for (int j=0; j < 5; j++) {
      OLED_update_buffer_single_byte(currentByte, (pgm_read_byte(&font5[font_table_index][j])));
      //printf("currentByte: %d, asciivalue: %d\n\r", currentByte, pgm_read_byte(&font5[font_table_index][j]));
      currentByte++;
    }
  }*/

}
