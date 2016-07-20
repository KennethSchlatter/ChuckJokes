#include <pebble.h>

#define MESSAGE_KEY_norrisJoke 0

static Window *window;
static TextLayer *joke;

static void js_ready_handler(void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "js_ready_handler called!");
}

static void new_joke_handler(DictionaryIterator *iter, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "NEW CHUCK NORRIS JOKE RECEIVED!");
  Tuple *joke_t = dict_find(iter, MESSAGE_KEY_norrisJoke);
  char *joke_n = joke_t->value->cstring;
  static char joke_buff[256];
  snprintf(joke_buff, sizeof(joke_buff), joke_n);
  text_layer_set_text(joke, joke_buff);
}

static void tap_handler(AccelAxisType axis, int32_t direction) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Detected Tap/Twist event!");
  DictionaryIterator *out_iter;
  AppMessageResult result = app_message_outbox_begin(&out_iter);
  if(result == APP_MSG_OK) {
    int value = 0;
    dict_write_int(out_iter, MESSAGE_KEY_norrisJoke, &value, sizeof(int), true);
    result = app_message_outbox_send();
    if(result != APP_MSG_OK) {
      APP_LOG(APP_LOG_LEVEL_ERROR, "Error sending the outbox: %d", (int)result);
    }
  }else{
    APP_LOG(APP_LOG_LEVEL_ERROR, "Error sending the outbox: %d", (int)result);
  }
}

static void window_load() {
  GRect bounds = layer_get_bounds(window_get_root_layer(window));
  joke = text_layer_create(bounds);
  text_layer_set_text_color(joke, GColorBlack);
  text_layer_set_text(joke, "Hello! Shake your wrist to see a random Chuck Norris joke brought to you by http://api.chucknorris.io");
}

static void window_unload() {
  text_layer_destroy(joke);
}

static void init() {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {.load = window_load, .unload = window_unload});
  window_stack_push(window, true);
  
  accel_tap_service_subscribe(tap_handler);
  
  app_message_register_inbox_received(new_joke_handler);
  app_message_open(app_message_inbox_size_maximum(), app_message_inbox_size_maximum());
  app_timer_register(3000, js_ready_handler, NULL);
}

static void deinit() {
  accel_tap_service_unsubscribe();
  
  window_destroy(window);
}

int main() {
  init();
  app_event_loop();
  deinit();
}