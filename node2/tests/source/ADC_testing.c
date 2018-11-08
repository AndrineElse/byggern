#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "../../drivers/include/ADC_driver.h"

void adc_test(){
  printf("ADC result: %u\n\r", adc_read());
}
