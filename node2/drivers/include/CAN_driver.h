struct CAN_msg {
  unsigned int id;
  uint8_t data[8];
  uint8_t length;
};

void CAN_init();
void CAN_send_msg(struct CAN_msg* msg);
struct CAN_msg CAN_receive_msg();
