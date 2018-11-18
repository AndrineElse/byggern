// void USART_Transmit_STXETX( uint8_t payload, uint8_t firstByte );
void USART_Transmit_Score(uint16_t score);
void USART_Transmit_Position(uint16_t position);
void USART_Transmit_Lives(uint16_t lives);
uint8_t* USART_Receive_STXETX();
