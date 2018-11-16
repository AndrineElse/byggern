#ifndef ADMUX
#define ADMUX (((char*) 0x7C)[0])
#define ADCSRA (((char*) 0x7A)[0])
#define ADCSRB (((char*) 0x7B)[0])
#define SMCR (((char*) 0x33)[0])
#endif

void adc_init();
uint16_t adc_read();
