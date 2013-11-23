#include <pebble.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TOTAL_WHO 2
#define TOTAL_MSG 3

static char *who_list[] = {"AA",	"BB"};
static char *num_list[] = {"111",	"222"};
static char *msg_list[] = {"####", 	"$$$$",		"%%%%"};
//// static char *tmp_list[] = {"OK", "No", "Ready%20to%20go?"}; 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static Window *window;
static TextLayer *who_layer;
static TextLayer *msg_layer;
static TextLayer *cmd_layer;

static AppSync sync;
static uint8_t sync_buffer[32];

int who_sel = 0;
int msg_sel = 0;
static char nam_text[64];
static char msg_text[64];

void request_mail_to_sms(void) {
}

void update_nam(void) {
    strcpy(nam_text, "To: "); strcat(nam_text, who_list[who_sel]);
    text_layer_set_text(who_layer, nam_text);
}

void update_msg(void) {
    strcpy(msg_text, "Msg: "); strcat(msg_text, msg_list[msg_sel]);
    text_layer_set_text(msg_layer, msg_text);
}

void up_single_click_handler(ClickRecognizerRef recognizer, void *context) {
    who_sel++; if (who_sel == TOTAL_WHO) who_sel = 0; update_nam();
}

void down_single_click_handler(ClickRecognizerRef recognizer, void *context) {
    msg_sel++; if (msg_sel == TOTAL_MSG) msg_sel = 0; update_msg();
}

void select_long_click_handler(ClickRecognizerRef recognizer, void *context) {
  	text_layer_set_text(cmd_layer, "Sending ...");
	request_mail_to_sms();
}

void select_long_click_release_handler(ClickRecognizerRef recognizer, void *context) {
}

void config_provider(Window *window) {
	window_single_click_subscribe(BUTTON_ID_UP, up_single_click_handler);
	window_single_click_subscribe(BUTTON_ID_DOWN, down_single_click_handler);
	window_long_click_subscribe(BUTTON_ID_SELECT, 700, select_long_click_handler, select_long_click_release_handler);
}

void sync_tuple_changed_callback(const uint32_t key, const Tuple* new_tuple, const Tuple* old_tuple, void* context) {
	switch (key) {
    	case 1: 
			strcpy(who_list[0], new_tuple->value->cstring);
			break;
		case 2:
			break;
	}
	who_sel++; if (who_sel == TOTAL_WHO) who_sel = 0; 
	update_nam(); update_msg();
}

static void sync_error_callback(DictionaryResult dict_error, AppMessageResult app_message_error, void* context) {
 	APP_LOG(APP_LOG_LEVEL_DEBUG, "app error %d", app_message_error);
}

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

	cmd_layer = text_layer_create(GRect(5, 110, 135, 40));
  	text_layer_set_text_color(cmd_layer, GColorBlack);
  	text_layer_set_background_color(cmd_layer, GColorClear);
  	text_layer_set_font(cmd_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  	layer_add_child(root_layer, text_layer_get_layer(cmd_layer));

    text_layer_set_text(who_layer, "To: -");
    text_layer_set_text(msg_layer, "Msg: -");
    text_layer_set_text(cmd_layer, "Send. Y/N?");
 
	window_set_click_config_provider(window, (ClickConfigProvider) config_provider);
	
	const int inbound_size = 64;
 	const int outbound_size = 64;
 	app_message_open(inbound_size, outbound_size);	
	
	Tuplet initial_values[] = {
    	TupletCString(1, who_list[0]),
    	TupletCString(2, who_list[1]),
    	TupletCString(3, num_list[0]),
    	TupletCString(4, num_list[1]),
    	TupletCString(5, msg_list[0]),
    	TupletCString(6, msg_list[1]),
    	TupletCString(7, msg_list[2])
 	};
	app_sync_init(&sync, sync_buffer, sizeof(sync_buffer), initial_values, ARRAY_LENGTH(initial_values), sync_tuple_changed_callback, sync_error_callback, NULL); 
}

void handle_deinit(void) {
  app_sync_deinit(&sync);
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