#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../include/ADCDriver2.h"

void adc_init(){
  // input channel selected
  // MUX bits in ADMUX and ADCSRB
  // Single conversion mode must be selected
  DDRF &= ~(1<<DDF0);
  ADMUX = 0x00; // ADC0 single ended input
  ADCSRB |= 0x00; // MUX5 low, set auto-trigger to trigger on flag high

  // ADC Enable, Start Conversion and Conversion complete interrupt
  ADCSRA = 0xA0; // Set ADEN ADC Enable
  SMCR = 0x02;   // Enter Noise reduction mode (or idle mode)
  ADCSRA |= 0x40;   // ADSC is set, start-conversion

}


uint16_t adc_read(){
  if (ADCSRA & 0x10) {
    uint16_t data = ((uint8_t*) 0x78)[0];
    data |= ((((uint8_t*) 0x79)[0]) << 8);
    return data;
  }
  return 0;
}
