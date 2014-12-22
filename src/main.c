#include "pebble.h"
#include "field.h"
#include "menu.h"

static Window *window;
static Field *field;
static CPattern pattern;
static uint16_t generation;
static bool is_evolution;

static AppTimer *timer;

#define DELAY_MANUAL_EVO                (100)
#define DELAY_AUTO_EVO_START_BY_MENU    (1000)
#define DELAY_AUTO_EVO_START_BY_UP      (0)
#define DELAY_AUTO_EVO_CLOCK            (1000)
#define DELAY_AUTO_EVO_OTHER            (200)
#define DELAY_AUTO_EVO_STOP             (1000)
#define DELAY_MENU                      (500)

static void field_init(CPattern _pattern);
static void menu_select_callback(CPattern _pattern);

static void timer_cancel(void) {
    if (timer != NULL) {
        app_timer_cancel(timer);
        timer = NULL;
    }
    tick_timer_service_unsubscribe();
}

static void timer_callback(void *data) {
    is_evolution = field_evolution(field);
    if (is_evolution == true) {
        timer = app_timer_register(DELAY_AUTO_EVO_OTHER, timer_callback, NULL);
    } else {
        psleep(DELAY_AUTO_EVO_STOP);
        menu_select_callback(pattern);
    }
    generation++;
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    if ((units_changed & MINUTE_UNIT) == MINUTE_UNIT) {
        field_init(CP_Clock);
    } else {
        switch (generation) {
        case 0: // fall down
        case 2:
            field_set_pattern(field, CP_None);
            break;
        case 1: // fall down
        case 3:
            field_set_pattern(field, CP_Clock);
            break;
        default:
            is_evolution = field_evolution(field);
            if (is_evolution == false) {
                field_init(CP_Clock);
            }
            break;
        }
        generation++;
    }
}

static void field_init(CPattern _pattern) {
    pattern = _pattern;
    generation = 0;
    is_evolution = true;
    field_set_pattern(field, pattern);
}

static void menu_select_callback(CPattern _pattern) {
    field_init(_pattern);
    if (pattern == CP_Clock) {
        tick_timer_service_subscribe(SECOND_UNIT | MINUTE_UNIT, tick_handler);
    } else {
        timer = app_timer_register(DELAY_AUTO_EVO_START_BY_MENU, timer_callback, NULL);
    }
}

static void up_single_click_handler(ClickRecognizerRef recognizer, void *context) {
    timer_cancel();
    if (pattern == CP_Clock) {
        tick_timer_service_subscribe(SECOND_UNIT | MINUTE_UNIT, tick_handler);
    } else {
        timer = app_timer_register(DELAY_AUTO_EVO_START_BY_UP, timer_callback, NULL);
    }
}

static void select_single_click_handler(ClickRecognizerRef recognizer, void *context) {
    timer_cancel();
    field_init(pattern);
}

static void select_long_click_handler(ClickRecognizerRef recognizer, void *context) {
    timer_cancel();
    (void)menu_create(pattern, menu_select_callback);
}

static void down_single_click_handler(ClickRecognizerRef recognizer, void *context) {
    timer_cancel();
    if (is_evolution == true) {
        is_evolution = field_evolution(field);
    }
}

static void config_provider(void *context) {
    window_single_click_subscribe(BUTTON_ID_UP, up_single_click_handler);
    window_single_click_subscribe(BUTTON_ID_SELECT, select_single_click_handler);
    window_long_click_subscribe(BUTTON_ID_SELECT, DELAY_MENU, select_long_click_handler, NULL);
    window_single_repeating_click_subscribe(BUTTON_ID_DOWN, DELAY_MANUAL_EVO, down_single_click_handler);
}

static void window_load(Window *window) {
    pattern = CP_Clock;
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
