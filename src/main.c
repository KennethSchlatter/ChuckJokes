#include <pebble.h>

static Window *joke_window;
static TextLayer *joke_layer;
static GFont joke_font;

enum{
  STATUS_KEY = 0,
  MESSAGE_KEY = 1
};

static void send_message(void){
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  dict_write_cstring(iter, MESSAGE_KEY, "I'm a Pebble!");
  dict_write_end(iter);
  app_message_outbox_send();
}

static void in_received_handler(DictionaryIterator *received, void *context){
  Tuple *tuple;
  tuple = dict_find(received, STATUS_KEY);
  if(tuple){
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Status: %d", (int)tuple->value->uint32);
  }
  tuple = dict_find(received, MESSAGE_KEY);
  if(tuple){
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Joke: %s", tuple->value->cstring);
    text_layer_set_text(joke_layer, tuple->value->cstring);
  }
  send_message();
}

static void in_dropped_handler(AppMessageResult reason, void *context){
  APP_LOG(APP_LOG_LEVEL_ERROR, "Inbox dropped");
}

static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context){
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox failed");
}

static void joke_window_load(){
  GRect bounds = layer_get_bounds(window_get_root_layer(joke_window));
  joke_layer = text_layer_create(bounds);
  text_layer_set_text(joke_layer, "Click select to fetch new joke!");
  text_layer_set_font(joke_layer, joke_font);
  #if defined(PBL_ROUND)
  text_layer_set_text_alignment(joke_layer, GTextAlignmentCenter);
  text_layer_enable_screen_text_flow_and_paging(joke_layer, 8);
  #endif
  layer_add_child(window_get_root_layer(joke_window), text_layer_get_layer(joke_layer));
  window_stack_push(joke_window, true);
}

static void joke_window_unload(){
  
}

static void init(void){
  joke_window = window_create();
  window_set_window_handlers(joke_window, (WindowHandlers) {.load = joke_window_load, .unload = joke_window_unload});
  joke_window_load();
  joke_font = fonts_get_system_font(FONT_KEY_GOTHIC_28);
  app_message_register_inbox_received(in_received_handler);
  app_message_register_inbox_dropped(in_dropped_handler);
  app_message_register_outbox_failed(out_failed_handler);
  const int inbox_size = 256;
  const int outbox_size = 128;
  app_message_open(inbox_size, outbox_size);
}

static void deinit(void){
  app_message_deregister_callbacks();
  joke_window_unload();
}

int main(void){
  init();
  app_event_loop();
  deinit();
}