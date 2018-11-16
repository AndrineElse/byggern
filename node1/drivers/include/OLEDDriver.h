void OLED_init();
void OLED_clear();
void OLED_home();
void OLED_goto_line(uint8_t line);
void OLED_clear_line(uint8_t line);
void OLED_pos(uint8_t row, uint8_t column);
void OLED_write_data(char c);
void OLED_print(char* string);
void OLED_buffer_print_line(char* string, uint8_t line, uint8_t inverseFlag);
void OLED_set_brightness(uint16_t lvl);
void OLED_write_command(uint8_t c);
void OLED_fill();
void OLED_write_char(uint8_t b);

/*buffer for oled starts at 0x1800
* uses 1024 addresses, each containing 8 bits
* should have final adress at 0x1bFF
* each mem is stored sequentially
* first 128 is each byte of first row, etc..
*/
void OLED_update_buffer_single_byte(uint16_t address, uint8_t data );

/*Similar to above, but write lots of bytes in sequence, useful for whole lines etc
*
*/
void OLED_update_buffer_array(uint16_t start_address, uint8_t* data, uint8_t data_amount);

/*Similar to above, assumes data array has 127 elements.
* line should be some value between 0 and 7
*/
void OLED_update_buffer_line(uint8_t line, uint8_t* data);


/*mode should be a number between 0 and 2, key:
* 0: Horizontal mode
* 1: Vertical mode
* 2: Page mode
*/
void OLED_set_access_mode(uint8_t mode);

/* sets coordinate bounds for horizontal addressing mode
* bounds should be within [0,127]
*/
void OLED_set_horizontal_bounds(uint8_t lower, uint8_t upper);

/* sets coordinate bounds for horizontal addressing mode
* bounds should be within [0,7]
*/
void OLED_set_vertical_bounds(uint8_t lower, uint8_t upper);

void OLED_init_buffer_mode();

void OLED_buffer_update_screen();

void OLED_buffer_clear();
void OLED_buffer_fill();
