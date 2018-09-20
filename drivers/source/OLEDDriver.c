#include <avr/io.h>
#include "../include/OLEDDriver.h"
#include <avr/pgmspace.h>
#include "../../fonts.h"

volatile char* command_address = (char*)0x1000;
volatile char* data_address = (char*)0x1200;

void OLED_write_command(uint8_t c) {
  // char* command_address = (char*)0x1000;
  command_address[0] = c;
}


void OLED_clear(){
  for (int i = 0; i < 8; i++){
    OLED_write_command(0b10110000 + i);
    for (int j =0; j < 128; j++){
      data_address[i] = 0x0;
    }
  }
}

void OLED_fill(){
  for (int i = 0; i < 8; i++){
    OLED_write_command(0b10110000 + i);
    for (int j =0; j < 128; j++){
      data_address[i] = 0xFF;
    }
  }
}

void OLED_init(){
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

}

void OLED_pos(uint16_t row, uint16_t column){
  OLED_write_command(0xb2);
  OLED_write_command(0x03);
  OLED_write_command(0x10);
}

void OLED_write_data(char c){

  data_address[0] = c;
}

void OLED_write_char(uint8_t b){
  if(b < 32){
    return;
  }
  uint8_t font_table_index = b - 32;

  for (int i=0; i < 8; i++) {
    OLED_write_data(pgm_read_byte(&font8[font_table_index][i]));
  }
}
