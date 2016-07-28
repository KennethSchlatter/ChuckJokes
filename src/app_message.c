/*
    This code is based off of Liam McMain's Movie Quotes API example watchapp.
    Liam's orriginal code can be found here: https://github.com/austinsparkleague/APIExampleOne/tree/master/movie_quotes .
    The API and menu image are from https://chucknorris.io . 
*/

#include <pebble.h>

static Window *s_window;	

static TextLayer *s_joke_layer;
static TextLayer *s_instructions;
static GFont s_text_font;

// Write message to buffer & send
static void send_message(void){
	DictionaryIterator *iter;
	
	app_message_outbox_begin(&iter);
	dict_write_cstring(iter, MESSAGE_KEY_message, "I'm a Pebble!");
	
	dict_write_end(iter);
  app_message_outbox_send();
}

// Called when a message is received from PebbleKitJS
static void in_received_handler(DictionaryIterator *received, void *context) {
	Tuple *tuple;
	
	tuple = dict_find(received, MESSAGE_KEY_status);
	if(tuple) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Status: %d", (int)tuple->value->uint32); 
	}
	
	tuple = dict_find(received, MESSAGE_KEY_message);
	if(tuple) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Message: %s", tuple->value->cstring);
    text_layer_set_text(s_joke_layer, tuple->value->cstring);
	}
  
}

// Called when an incoming message from PebbleKitJS is dropped
static void in_dropped_handler(AppMessageResult reason, void *context) {	
}

// Called when PebbleKitJS does not acknowledge receipt of a message
static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
}

void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  send_message();
}

void config_provider(Window *window) {
 // single click / repeat-on-hold config:
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

static void hide_instructions(){
  text_layer_destroy(s_instructions);
}

static void init(void) {
	s_window = window_create();
  window_set_click_config_provider(s_window, (ClickConfigProvider) config_provider);
	window_stack_push(s_window, true);
  send_message();
  
  s_text_font = fonts_get_system_font(FONT_KEY_GOTHIC_18);

  Layer *window_layer = window_get_root_layer(s_window);
  GRect bounds = layer_get_bounds(window_layer);
  
  s_joke_layer = text_layer_create(bounds);
  text_layer_set_text(s_joke_layer, "\n Loading...");
  text_layer_set_font(s_joke_layer, s_text_font);
  layer_add_child(window_layer, text_layer_get_layer(s_joke_layer));
  text_layer_set_text_alignment(s_joke_layer, GTextAlignmentCenter);
  text_layer_enable_screen_text_flow_and_paging(s_joke_layer, 8);
	
//  GRect custom_bounds = GRect(bounds.origin.x, bounds.size.h/2, bounds.size.w, bounds.size.h/2);
  s_instructions = text_layer_create(bounds);
  text_layer_set_text(s_instructions, "\n Press the Select button to get a new Chuck Norris joke");
  text_layer_set_font(s_instructions, s_text_font);
  text_layer_set_text_color(s_instructions, GColorWhite);
  text_layer_set_background_color(s_instructions, GColorBlack);
  layer_add_child(window_layer, text_layer_get_layer(s_instructions));
  text_layer_set_text_alignment(s_instructions, GTextAlignmentCenter);
  text_layer_enable_screen_text_flow_and_paging(s_instructions, 8);
  app_timer_register(3000, hide_instructions, NULL);
	
	// Register AppMessage handlers
	app_message_register_inbox_received(in_received_handler); 
	app_message_register_inbox_dropped(in_dropped_handler); 
	app_message_register_outbox_failed(out_failed_handler);

  // Initialize AppMessage inbox and outbox buffers with a suitable size
  const int inbox_size = 128;
  const int outbox_size = 128;
	app_message_open(inbox_size, outbox_size);
}

static void deinit(void) {
	app_message_deregister_callbacks();
	window_destroy(s_window);
}

int main( void ) {
	init();
	app_event_loop();
	deinit();
}