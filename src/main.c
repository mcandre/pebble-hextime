#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;

static void main_window_load(Window *window) {
  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(0, 55, 144, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text(s_time_layer, "f_ff");

  // Improve the layout to be more like a watchface
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
}

static void main_window_unload(Window *window) {
    // Destroy TextLayer
    text_layer_destroy(s_time_layer);
}

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[] = "f_ff";

  int seconds = (unsigned int)(
    (tick_time->tm_hour * 3600 + tick_time->tm_min * 60 + tick_time->tm_sec) * 65536.0 / 86400.0
  );

  int hhour = seconds / 4096;

  int hmin = (seconds % 4096) / 16;

  snprintf(buffer, 5, "%x_%02x", hhour, hmin);

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
}

static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);

  // Make sure the time is displayed from the start
  update_time();
}

static void deinit() {
    // Destroy Window
    window_destroy(s_main_window);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}