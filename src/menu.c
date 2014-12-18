#include <pebble.h>
#include "menu.h"

typedef struct menu {
    Window *window;
    MenuLayer *menu_layer;
} Menu;

static void s_window_load(Window *window);
static void s_window_unload(Window *window);

Menu *menu_create(void) {
    Menu *menu = NULL;

    menu = calloc(1, sizeof(Menu));
    if (menu != NULL) {
        Window *window = window_create();
        if (window != NULL) {
            menu->window = window;
 
            window_set_background_color(window, GColorWhite);
            window_set_user_data(window, (void*)menu);
            window_set_window_handlers(window, (WindowHandlers) {
                .load = s_window_load,
                .unload = s_window_unload,
            });
            window_stack_push(window, true /* Animated */);
        } else {
            menu_destroy(menu);
            menu = NULL;
        }
    }
    return menu;
}

void menu_destroy(Menu *menu) {
    if (menu == NULL) {
        return;
    }
    if (menu->window != NULL) {
        window_stack_pop(true);
        window_destroy(menu->window);
    }
    free(menu);
}

static void s_select_single_click_handler(ClickRecognizerRef recognizer, void *context) {
    Window *window = (Window*)context;
    Menu *menu = window_get_user_data(window);
    menu_destroy(menu);
}

static void s_config_provider(void *context) {
    window_single_click_subscribe(BUTTON_ID_SELECT, s_select_single_click_handler);
}

static void s_window_load(Window *window) {
    window_set_click_config_provider(window, s_config_provider);
}

static void s_window_unload(Window *window) {
    // do nothing
}