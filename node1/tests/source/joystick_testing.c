#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "../include/joystick_testing.h"

void joystick_test(void){
  volatile char* adc_ch1 = (char*) 0x1400;
  uint16_t ext_ram_size = 0x400;
  int i = 5;


  while(1){
    adc_ch1[0] = i;
    _delay_ms(60);
    //send 4+ channel til adc
    //vent 60mikro-ish
    //les på adc
    uint8_t retreived_value = adc_ch1[0];
    printf("Adresse: %d , data: %d \n\r",0x1400 + i, retreived_value);
  }
}
