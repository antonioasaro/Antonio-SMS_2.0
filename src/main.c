#include <pebble.h>
#include "util.h"


///////////////////////////////////////////////
#define TOTAL_WHO 4
#define TOTAL_MSG 5

static char frm[32];
static char who_list[TOTAL_WHO][32];
static char num_list[TOTAL_WHO][32];
static char msg_list[TOTAL_MSG][32];
static char sts[32];
///////////////////////////////////////////////

static AppTimer *timer;
static AppSync sync;
static uint8_t sync_buffer[512];
static DictionaryIterator dict_iter, *iter = &dict_iter;

static Window *window;
static TextLayer *frm_layer;
static TextLayer *who_layer;
static TextLayer *msg_layer;
static TextLayer *cmd_layer;
static bool sending = false;
static int who_sel = 0;
static int msg_sel = 0;

static void handle_timer(void *data);

void request_mail_to_sms(void) {
    static char *frmptr = frm;
	static char num[32], *numptr = num;
    static char msg[32], *msgptr = msg;
	static char *msg_rpl = NULL;

	if (sending) return;
  	text_layer_set_text(cmd_layer, "Sending ...");
	sending = true;
	
	strcpy(num, num_list[who_sel]);
	msg_rpl = replace_char(msg_list[msg_sel], ' ', "%20");
	strcpy(msg, msg_rpl);
	free(msg_rpl); msg_rpl = NULL;

	app_message_outbox_begin(&iter);
	Tuplet rqs_val = TupletCString(98, "request_sms");
	Tuplet frm_val = TupletCString(50, frmptr);
	Tuplet num_val = TupletCString(51, numptr);
	Tuplet msg_val = TupletCString(52, msgptr);
	dict_write_tuplet(iter, &rqs_val);
	dict_write_tuplet(iter, &frm_val);
	dict_write_tuplet(iter, &num_val);
	dict_write_tuplet(iter, &msg_val);
	app_message_outbox_send();
}


void update_frm(void) {
    static char frm_text[64];
    strcpy(frm_text, "Frm: "); strcat(frm_text, frm);
    text_layer_set_text(frm_layer, frm_text);
}

void update_who(void) {
    static char nam_text[64];
    strcpy(nam_text, "To: "); strcat(nam_text, who_list[who_sel]);
    text_layer_set_text(who_layer, nam_text);
}

void update_msg(void) {
    static char msg_text[64];
    strcpy(msg_text, "Msg: "); strcat(msg_text, msg_list[msg_sel]);
    text_layer_set_text(msg_layer, msg_text);
}


void up_single_click_handler(ClickRecognizerRef recognizer, void *context) {
    who_sel++; if (who_sel == TOTAL_WHO) who_sel = 0; update_who();
}

void down_single_click_handler(ClickRecognizerRef recognizer, void *context) {
    msg_sel++; if (msg_sel == TOTAL_MSG) msg_sel = 0; update_msg();
}

void select_long_click_handler(ClickRecognizerRef recognizer, void *context) {
	request_mail_to_sms();
    timer = app_timer_register(5000, &handle_timer, NULL);
}

void select_long_click_release_handler(ClickRecognizerRef recognizer, void *context) {
}

void config_provider(Window *window) {
	window_single_click_subscribe(BUTTON_ID_UP, up_single_click_handler);
	window_single_click_subscribe(BUTTON_ID_DOWN, down_single_click_handler);
	window_long_click_subscribe(BUTTON_ID_SELECT, 1000, select_long_click_handler, select_long_click_release_handler);
}


void sync_tuple_changed_callback(const uint32_t key, const Tuple* new_tuple, const Tuple* old_tuple, void* context) {
	
	switch (key) {
		case 1:  strcpy(frm,         new_tuple->value->cstring); break; 	
		case 2:  strcpy(who_list[0], new_tuple->value->cstring); break; 	
		case 3:  strcpy(num_list[0], new_tuple->value->cstring); break; 	
		case 4:  strcpy(who_list[1], new_tuple->value->cstring); break; 	
		case 5:  strcpy(num_list[1], new_tuple->value->cstring); break; 	
		case 6:  strcpy(who_list[2], new_tuple->value->cstring); break; 	
		case 7:  strcpy(num_list[2], new_tuple->value->cstring); break; 	
		case 8:  strcpy(who_list[3], new_tuple->value->cstring); break; 	
		case 9:  strcpy(num_list[3], new_tuple->value->cstring); break; 	
		case 10: strcpy(msg_list[0], new_tuple->value->cstring); break; 	
		case 11: strcpy(msg_list[1], new_tuple->value->cstring); break; 	
		case 12: strcpy(msg_list[2], new_tuple->value->cstring); break; 	
		case 13: strcpy(msg_list[3], new_tuple->value->cstring); break; 	
		case 14: strcpy(msg_list[4], new_tuple->value->cstring); break; 	
		case 99: 
			strcpy(sts,         new_tuple->value->cstring); 
			if (sending) text_layer_set_text(cmd_layer, sts);
			break; 	
    }
	update_frm(); 
	update_who(); 
	update_msg();
}


static void sync_error_callback(DictionaryResult dict_error, AppMessageResult app_message_error, void* context) {
 	APP_LOG(APP_LOG_LEVEL_DEBUG, "app error %d", app_message_error);
}

void out_sent_handler(DictionaryIterator *sent, void *context) {
}

void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
}

void in_received_handler(DictionaryIterator *received, void *context) {
}

void in_dropped_handler(AppMessageResult reason, void *context) {
}


static void handle_timer(void *data)
{
    sending = false;
	text_layer_set_text(cmd_layer, "Send. Y/N? -->");
}


void handle_init(void) {
	window = window_create();
	window_stack_push(window, true);
  	Layer *root_layer = window_get_root_layer(window);

#ifdef PBL_ROUND
	frm_layer = text_layer_create(GRect(25, 36,  135, 30));
#else
	frm_layer = text_layer_create(GRect(5, 20,  135, 30));
#endif
#ifdef PBL_BW
  	text_layer_set_text_color(frm_layer, GColorBlack);
#else
	  	text_layer_set_text_color(frm_layer, GColorRed);
#endif
  	text_layer_set_background_color(frm_layer, GColorClear);
  	text_layer_set_font(frm_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  	layer_add_child(root_layer, text_layer_get_layer(frm_layer));

	
#ifdef PBL_ROUND
	cmd_layer = text_layer_create(GRect(25, 66, 135, 30));
#else
	cmd_layer = text_layer_create(GRect(5, 60, 135, 30));
#endif 
#ifdef PBL_BW	
  	text_layer_set_text_color(cmd_layer, GColorBlack);
#else	
	text_layer_set_text_color(cmd_layer, GColorDarkGreen);
#endif	
  	text_layer_set_background_color(cmd_layer, GColorClear);
  	text_layer_set_font(cmd_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  	layer_add_child(root_layer, text_layer_get_layer(cmd_layer));

	
#ifdef PBL_ROUND
	who_layer = text_layer_create(GRect(25, 96,  135, 30));
#else	
	who_layer = text_layer_create(GRect(5, 100,  135, 30));
#endif 
#ifdef PBL_BW	
  	text_layer_set_text_color(who_layer, GColorBlack);
#else
	text_layer_set_text_color(who_layer, GColorBlue);
#endif	
  	text_layer_set_background_color(who_layer, GColorClear);
  	text_layer_set_font(who_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  	layer_add_child(root_layer, text_layer_get_layer(who_layer));


#ifdef PBL_ROUND	
	msg_layer = text_layer_create(GRect(25, 116,  135, 30));
#else	
	msg_layer = text_layer_create(GRect(5, 120,  135, 30));
#endif 
#ifdef PBL_BW	
  	text_layer_set_text_color(msg_layer, GColorBlack);
#else
	text_layer_set_text_color(msg_layer, GColorBlue);
#endif	
  	text_layer_set_background_color(msg_layer, GColorClear);
  	text_layer_set_font(msg_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  	layer_add_child(root_layer, text_layer_get_layer(msg_layer));

    text_layer_set_text(who_layer, "To:");
    text_layer_set_text(msg_layer, "Msg:");
    text_layer_set_text(cmd_layer, "Send. Y/N? -->");
 
	window_set_click_config_provider(window, (ClickConfigProvider) config_provider);
	
	const int inbound_size = APP_MESSAGE_INBOX_SIZE_MINIMUM;
 	const int outbound_size = APP_MESSAGE_OUTBOX_SIZE_MINIMUM;
 	app_message_open(512, 512);	
	
	Tuplet initial_values[] = {
    	TupletCString(1,  "        "),
    	TupletCString(2,  "        "),
    	TupletCString(3,  "        "),
    	TupletCString(4,  "        "),
    	TupletCString(5,  "        "),
    	TupletCString(6,  "        "),
    	TupletCString(7,  "        "),
    	TupletCString(8,  "        "),
    	TupletCString(9,  "        "),
    	TupletCString(10, "        "),
    	TupletCString(11, "        "),
    	TupletCString(12, "        "),
    	TupletCString(13, "        "),
    	TupletCString(14, "        "),
		TupletCString(99, "        ")
  	};
	app_sync_init(&sync, sync_buffer, sizeof(sync_buffer), initial_values, ARRAY_LENGTH(initial_values), sync_tuple_changed_callback, sync_error_callback, NULL); 

 	psleep(2000);
	app_message_outbox_begin(&iter);
	Tuplet cmd_val = TupletCString(98, "rd_settings");
	dict_write_tuplet(iter, &cmd_val);
	app_message_outbox_send();
}


void handle_deinit(void) {
  	app_sync_deinit(&sync);
  	text_layer_destroy(frm_layer);
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
