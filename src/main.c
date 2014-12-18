#include "pebble.h"
#include "field.h"

static Window *window;
static Field *field;

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
    field_set_time(field, time(NULL));
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
    field_evolution(field);
}

static void config_provider(void *context) {
    window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
    window_single_repeating_click_subscribe(BUTTON_ID_DOWN, 100, down_click_handler);
}

static void init() {
    window = window_create();
    window_stack_push(window, true /* Animated */);
    window_set_background_color(window, GColorBlack);

    Layer *window_layer = window_get_root_layer(window);
    field = field_create(layer_get_frame(window_layer));
    if (field != NULL) {
        window_set_click_config_provider(window, config_provider);
        layer_add_child(window_layer, field_get_layer(field));
    }
}

static void deinit() {
    field_destroy(field);
    window_destroy(window);
}

int main(void) {
    init();
    app_event_loop();
    deinit();
}
