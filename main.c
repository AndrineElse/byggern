#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 16000000

void main(){
  DDRA = 0xFF;
  PORTA = 0;
  while(1){
    PORTA = 0xFF;
    _delay_ms(10);
    PORTA = 0;
    _delay_ms(10);
  }
  return;
}
