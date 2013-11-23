#include <pebble.h>

static Window *window;
static TextLayer *who_layer;
static TextLayer *msg_layer;
static TextLayer *cmd_layer;

int who_sel = 0;
int msg_sel = 0;
static char nam_text[64];
static char msg_text[64];


void handle_init(void) {
	window = window_create();
	window_stack_push(window, true);

  	Layer *root_layer = window_get_root_layer(window);

  	who_layer = text_layer_create(GRect(5, 10,  135, 30));
  	text_layer_set_text_color(who_layer, GColorBlack);
  	text_layer_set_background_color(who_layer, GColorClear);
  	text_layer_set_font(who_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  	layer_add_child(root_layer, text_layer_get_layer(who_layer));

	msg_layer = text_layer_create(GRect(5, 40,  135, 60));
  	text_layer_set_text_color(msg_layer, GColorBlack);
  	text_layer_set_background_color(msg_layer, GColorClear);
  	text_layer_set_font(msg_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  	layer_add_child(root_layer, text_layer_get_layer(msg_layer));

	cmd_layer = text_layer_create(GRect(5, 40,  135, 60));
  	text_layer_set_text_color(cmd_layer, GColorBlack);
  	text_layer_set_background_color(cmd_layer, GColorClear);
  	text_layer_set_font(cmd_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  	layer_add_child(root_layer, text_layer_get_layer(cmd_layer));
 }

void handle_deinit(void) {
  text_layer_destroy(who_layer);
  text_layer_destroy(msg_layer);
  text_layer_destroy(cmd_layer);
  window_destroy(window);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}