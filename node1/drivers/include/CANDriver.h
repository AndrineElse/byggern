struct CAN_msg {
  unsigned int id;
  uint8_t data[8];
  uint8_t length;
};

void CAN_init();
void send_CAN_msg(struct CAN_msg* msg);
struct CAN_msg receive_msg();
