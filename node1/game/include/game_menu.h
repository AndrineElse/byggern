struct Node {
  struct Node* parent;
  char* options[5];
  struct Node* option_nodes[5];
  char* description;
  uint8_t num_options;
};

void game_menu_loop(struct Node* start_node, Joystick_offset offset);
void print_game_node(struct Node* node, uint8_t selected_option);
void game_menu_init(struct Node* main_menu_node);
void print_game_node_using_buffer(struct Node* node, uint8_t selected_option);
