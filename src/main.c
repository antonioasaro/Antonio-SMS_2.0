#include <pebble.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TOTAL_WHO 7
#define TOTAL_MSG 8
const char *nam_list[] = {"Antonio",                   "Lori",                      "Natalie",               "Alex",                  "Vince",                          "Dave",                      "Tim"};
const char *who_list[] = {"4165621384@sms.rogers.com", "4162713650@sms.rogers.com", "4167974863@rogers.com", "4165096453@rogers.com", "4168804473@msg.koodomobile.com", "4164180128@sms.rogers.com", "tpers@antelsystems.com"};
const char *msg_list[] = {"OK", "No", "Lunch?", "Ready%20to%20go?", "Just%20a%20sec", "Running%20late", "On%20my%20way%20home", "Busy,%20call%20you%20later"};    // repace spaces w/ "%20"
const char *tmp_list[] = {"OK", "No", "Lunch?", "Ready to go?",     "Just a sec",     "Running late",   "On my way home",       "Busy, call you later"};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
static Window *window;
static TextLayer *who_layer;
static TextLayer *msg_layer;
static TextLayer *cmd_layer;

int who_sel = 0;
int msg_sel = 0;
static char nam_text[64];
static char msg_text[64];

void request_mail_to_sms (void) {
}

void up_single_click_handler(ClickRecognizerRef recognizer, void *context) {
   who_sel++; if (who_sel == TOTAL_WHO) who_sel = 0;
    strcpy(nam_text, "To: "); strcat(nam_text, nam_list[who_sel]);
    text_layer_set_text(who_layer, nam_text);
}

void down_single_click_handler(ClickRecognizerRef recognizer, void *context) {
    msg_sel++; if (msg_sel == TOTAL_MSG) msg_sel = 0;
    strcpy(msg_text, "Msg: "); strcat(msg_text, tmp_list[msg_sel]);
    text_layer_set_text(msg_layer, msg_text);
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
	
	 window_set_click_config_provider(window, (ClickConfigProvider) config_provider);
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