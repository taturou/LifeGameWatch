#include "pebble.h"
#include "field.h"
#include "menu.h"

static Window *window;
static Field *field;

static AppTimer *timer;
#define DELAY_AUTO_EVO    (200)
#define DELAY_MENU        (500)

static void timer_cancel(void) {
    if (timer != NULL) {
        app_timer_cancel(timer);
        timer = NULL;
    }
}

static void timer_callback(void *data) {
    field_evolution(field);
    timer = app_timer_register(DELAY_AUTO_EVO, timer_callback, NULL);
}

static void up_single_click_handler(ClickRecognizerRef recognizer, void *context) {
    timer_cancel();
    timer = app_timer_register(DELAY_AUTO_EVO, timer_callback, NULL);
}

static void select_single_click_handler(ClickRecognizerRef recognizer, void *context) {
    timer_cancel();
    field_set_time(field, time(NULL));
}

static void select_long_click_handler(ClickRecognizerRef recognizer, void *context) {
    timer_cancel();
    menu_create();
}

static void down_single_click_handler(ClickRecognizerRef recognizer, void *context) {
    timer_cancel();
    field_evolution(field);
}

static void config_provider(void *context) {
    window_single_click_subscribe(BUTTON_ID_UP, up_single_click_handler);
    window_single_click_subscribe(BUTTON_ID_SELECT, select_single_click_handler);
    window_long_click_subscribe(BUTTON_ID_SELECT, DELAY_MENU, select_long_click_handler, NULL);
    window_single_repeating_click_subscribe(BUTTON_ID_DOWN, 100, down_single_click_handler);
}

static void window_load(Window *window) {
    timer = NULL;

    Layer *window_layer = window_get_root_layer(window);
    field = field_create(layer_get_frame(window_layer));
    if (field != NULL) {
        window_set_click_config_provider(window, config_provider);
        layer_add_child(window_layer, field_get_layer(field));
    }
}

static void window_unload(Window *window) {
    field_destroy(field);
}

static void init() {
    window = window_create();
    window_set_background_color(window, GColorBlack);
    window_set_window_handlers(window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });
    window_stack_push(window, true /* Animated */);
}

static void deinit() {
    window_destroy(window);
}

int main(void) {
    init();
    app_event_loop();
    deinit();
}
