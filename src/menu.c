#include <pebble.h>
#include "menu.h"

#define NUM_MENU_SECTIONS 2
#define NUM_FIRST_MENU_ITEMS 1
#define NUM_SECOND_MENU_ITEMS 2

typedef struct menu {
    Window *window;
    MenuLayer *layer;
    MenuSelectCallback callback;
} Menu;

static void s_window_load(Window *window);
static void s_window_unload(Window *window);

Menu *menu_create(MenuSelectCallback callback) {
    Menu *menu = NULL;

    menu = calloc(1, sizeof(Menu));
    if (menu != NULL) {
        menu->callback = callback;

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

static uint16_t s_menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
    return NUM_MENU_SECTIONS;
}

static uint16_t s_menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
    uint16_t num = 0;

    switch (section_index) {
    case 0:
        num = NUM_FIRST_MENU_ITEMS;
        break;
    case 1:
        num = NUM_SECOND_MENU_ITEMS;
        break;
    default:
        break;
    }
    return num;
}

static int16_t s_menu_get_cell_hight_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
    return 35;
}
    
static int16_t s_menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
    // This is a define provided in pebble.h that you may use for the default height
    return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static void s_menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
    switch (section_index) {
    case 0:
        menu_cell_basic_header_draw(ctx, cell_layer, "Special pattern");
        break;
    case 1:
        menu_cell_basic_header_draw(ctx, cell_layer, "Popular pattern");
        break;
    default:
        break;
    }
}

static void s_menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
    switch (cell_index->section) {
    case 0:
        switch (cell_index->row) {
        case 0:
            menu_cell_title_draw(ctx, cell_layer, "Clock");
            break;
        default:
            break;
        }
        break;
    case 1:
        switch (cell_index->row) {
        case 0:
            menu_cell_title_draw(ctx, cell_layer, "Glider");
            break;
        case 1:
            menu_cell_title_draw(ctx, cell_layer, "Lightweight spaceship ");
            break;
        default:
            break;
        }
    }
}

static void s_menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
    Menu *menu = (Menu*)data;
    Pattern pattern = None;

    switch (cell_index->section) {
    case 0:
        switch (cell_index->row) {
        case 0:
            pattern = Clock;
            break;
        default:
            break;
        }
        break;
    case 1:
        switch (cell_index->row) {
        case 0:
            pattern = Glider;
            break;
        case 1:
            pattern = LWSS;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    (*menu->callback)(pattern);
    menu_destroy(menu);
}

static void s_window_load(Window *window) {
    Menu *menu = window_get_user_data(window);
    
    Layer *window_layer = window_get_root_layer(window);

    menu->layer = menu_layer_create(layer_get_frame(window_layer));
    menu_layer_set_callbacks(menu->layer, (void*)menu, (MenuLayerCallbacks){
        .get_num_sections = s_menu_get_num_sections_callback,
        .get_num_rows = s_menu_get_num_rows_callback,
        .get_cell_height = s_menu_get_cell_hight_callback,
        .get_header_height = s_menu_get_header_height_callback,
        .draw_header = s_menu_draw_header_callback,
        .draw_row = s_menu_draw_row_callback,
        .select_click = s_menu_select_callback,
    });
    menu_layer_set_click_config_onto_window(menu->layer, window);
    layer_add_child(window_layer, menu_layer_get_layer(menu->layer));
}

static void s_window_unload(Window *window) {
    // do nothing
}