void solenoid_init();
uint8_t button_pressed();
void solenoid_update_status(uint8_t* button_flag, uint16_t* timer);
void solenoid_fire(uint8_t* button_flag, uint16_t* timer);
