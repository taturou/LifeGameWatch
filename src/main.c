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

static void s_field_init(CPattern _pattern);
static void s_menu_select_callback(CPattern _pattern);

static void s_timer_cancel(void) {
    if (timer != NULL) {
        app_timer_cancel(timer);
        timer = NULL;
    }
    tick_timer_service_unsubscribe();
}

static void s_timer_callback(void *data) {
    is_evolution = field_evolution(field);
    if (is_evolution == true) {
        timer = app_timer_register(DELAY_AUTO_EVO_OTHER, s_timer_callback, NULL);
    } else {
        psleep(DELAY_AUTO_EVO_STOP);
        s_menu_select_callback(pattern);
    }
    generation++;
}

static void s_tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    if ((units_changed & MINUTE_UNIT) == MINUTE_UNIT) {
        s_field_init(CP_Clock);
    } else {
        switch (generation) {
        case 0: // fall down
        case 2:
            field_set_pattern(field, CP_None);
            is_evolution = true;
            break;
        case 1: // fall down
        case 3:
            field_set_pattern(field, CP_Clock);
            is_evolution = true;
            break;
        default:
            is_evolution = field_evolution(field);
            break;
        }
        if (is_evolution == true) {
            generation++;
        } else {
            s_field_init(CP_Clock);
        }
    }
}

static void s_field_init(CPattern _pattern) {
    pattern = _pattern;
    generation = 0;
    is_evolution = true;
    field_set_pattern(field, pattern);
}

static void s_menu_select_callback(CPattern _pattern) {
    (void)field_reset_cell_size(field, (rand() % 6) + 2);
    field_draw_grid(field, rand() % 2);
    
    s_field_init(_pattern);
    if (pattern == CP_Clock) {
        tick_timer_service_subscribe(SECOND_UNIT | MINUTE_UNIT, s_tick_handler);
    } else {
        timer = app_timer_register(DELAY_AUTO_EVO_START_BY_MENU, s_timer_callback, NULL);
    }
}

static void s_up_single_click_handler(ClickRecognizerRef recognizer, void *context) {
    s_timer_cancel();
    if (pattern == CP_Clock) {
        tick_timer_service_subscribe(SECOND_UNIT | MINUTE_UNIT, s_tick_handler);
    } else {
        timer = app_timer_register(DELAY_AUTO_EVO_START_BY_UP, s_timer_callback, NULL);
    }
}

static void s_select_single_click_handler(ClickRecognizerRef recognizer, void *context) {
    s_timer_cancel();
    s_field_init(pattern);
}

static void s_select_long_click_handler(ClickRecognizerRef recognizer, void *context) {
    s_timer_cancel();
    (void)menu_create(pattern, s_menu_select_callback);
}

static void s_down_single_click_handler(ClickRecognizerRef recognizer, void *context) {
    s_timer_cancel();
    if (is_evolution == true) {
        is_evolution = field_evolution(field);
    }
}

static void config_provider(void *context) {
    window_single_click_subscribe(BUTTON_ID_UP, s_up_single_click_handler);
    window_single_click_subscribe(BUTTON_ID_SELECT, s_select_single_click_handler);
    window_long_click_subscribe(BUTTON_ID_SELECT, DELAY_MENU, s_select_long_click_handler, NULL);
    window_single_repeating_click_subscribe(BUTTON_ID_DOWN, DELAY_MANUAL_EVO, s_down_single_click_handler);
}

static void s_window_load(Window *window) {
    pattern = CP_Clock;
    timer = NULL;

    Layer *window_layer = window_get_root_layer(window);
    field = field_create(layer_get_frame(window_layer));
    if (field != NULL) {
        window_set_click_config_provider(window, config_provider);
        layer_add_child(window_layer, field_get_layer(field));
        s_menu_select_callback(CP_Clock);
    }
}

static void s_window_unload(Window *window) {
    field_destroy(field);
}

static void s_init() {
    srand(time(NULL));
    
    window = window_create();
    window_set_background_color(window, GColorBlack);
    window_set_window_handlers(window, (WindowHandlers) {
        .load = s_window_load,
        .unload = s_window_unload,
    });
    window_stack_push(window, true /* Animated */);
}

static void s_deinit() {
    window_destroy(window);
}

int main(void) {
    s_init();
    app_event_loop();
    s_deinit();
}
