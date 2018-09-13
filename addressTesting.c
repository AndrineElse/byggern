#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include "addressTesting.h"

void addressTest(void){
  DDRC = 0xFF;
  PORTC = 0x0;

}
