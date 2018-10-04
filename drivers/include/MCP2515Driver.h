#define CAN_CS  4

uint8_t mcp2515_read(uint8_t address);
uint8_t mcp2515_init();
void mcp2515_write(uint8_t address, uint8_t data);
void mcp2515_request_to_send();
void mcp2515_bit_modify(uint8_t address, uint8_t mask, uint8_t data);
void mcp2515_reset();
uint8_t mcp2515_read_status();
void mcp2515_read_store_pointer(uint8_t address, uint8_t* value);
