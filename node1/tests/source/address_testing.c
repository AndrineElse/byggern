#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include "../include/address_testing.h"

void address_test(void){
  DDRC = 0xFF;
  PORTC = 0x0;

}
