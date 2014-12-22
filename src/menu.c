#include <pebble.h>
#include "menu.h"

#define NUM_MENU_SECTIONS 2
#define NUM_FIRST_MENU_ITEMS 1
#define NUM_SECOND_MENU_ITEMS 3

typedef struct menu {
    Window *window;
    MenuLayer *layer;
    GBitmap *icons[MAX_CPATTERN];
    MenuIndex selected_index;
    MenuSelectCallback callback;
} Menu;

static void s_window_load(Window *window);
static void s_window_unload(Window *window);
static MenuIndex s_menu_get_index_from_pattern(CPattern pattern);

Menu *menu_create(CPattern now_pattern, MenuSelectCallback callback) {
    Menu *menu = NULL;

    menu = calloc(1, sizeof(Menu));
    if (menu != NULL) {
        menu->callback = callback;

        Window *window = window_create();
        if (window != NULL) {
            menu->window = window;
 
            // calc index
            menu->selected_index = s_menu_get_index_from_pattern(now_pattern);
            
            // create icons
            menu->icons[CP_Clock] = gbitmap_create_with_resource(RESOURCE_ID_MENU_ICON_CLOCK);
            menu->icons[CP_Glider] = gbitmap_create_with_resource(RESOURCE_ID_MENU_ICON_GLIDER);
            menu->icons[CP_LWSaceship] = gbitmap_create_with_resource(RESOURCE_ID_MENU_ICON_LWSS);
            menu->icons[CP_RRntomino] = gbitmap_create_with_resource(RESOURCE_ID_MENU_ICON_RPENT);

            // init window
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
    const uint16_t num_rows[NUM_MENU_SECTIONS] = {
        NUM_FIRST_MENU_ITEMS,
        NUM_SECOND_MENU_ITEMS
    };

    return num_rows[section_index];
}

#if 0
static int16_t s_menu_get_cell_hight_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
    return 35;
}
#endif
    
static int16_t s_menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
    // This is a define provided in pebble.h that you may use for the default height
    return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static void s_menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
    const char *titles[NUM_MENU_SECTIONS] = {
        "Special pattern",
        "Popular pattern"
    };
    menu_cell_basic_header_draw(ctx, cell_layer, titles[section_index]);
}

static void s_menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
    Menu *menu = (Menu*)data;
    struct basic_cell {
        char *title;
        char *sub_title;
        GBitmap *icon;
    };
    const struct basic_cell cells1[NUM_FIRST_MENU_ITEMS] = {
        {"Clock", "The present time", menu->icons[CP_Clock]}
    };
    const struct basic_cell cells2[NUM_SECOND_MENU_ITEMS] = {
        {"Glider", "Most popular glider", menu->icons[CP_Glider]},
        {"Spaceship", "Lightweight spaceship", menu->icons[CP_LWSaceship]},
        {"R-pentomino", "Not stabilize", menu->icons[CP_RRntomino]}
    };
    const struct basic_cell *cells[NUM_MENU_SECTIONS] = {
        cells1,
        cells2
    };
    const struct basic_cell *cell = &cells[cell_index->section][cell_index->row];
    menu_cell_basic_draw(ctx, cell_layer, cell->title, cell->sub_title, cell->icon);
}

static void s_menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
    Menu *menu = (Menu*)data;
    const CPattern patterns1[NUM_FIRST_MENU_ITEMS] = {
        CP_Clock
    };        
    const CPattern patterns2[NUM_SECOND_MENU_ITEMS] = {
        CP_Glider,
        CP_LWSaceship,
        CP_RRntomino 
    };        
    const CPattern *patterns[NUM_MENU_SECTIONS] = {
        patterns1,
        patterns2
    };
    const CPattern pattern = patterns[cell_index->section][cell_index->row];

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
        // .get_cell_height = s_menu_get_cell_hight_callback,
        .get_header_height = s_menu_get_header_height_callback,
        .draw_header = s_menu_draw_header_callback,
        .draw_row = s_menu_draw_row_callback,
        .select_click = s_menu_select_callback,
    });
    menu_layer_set_click_config_onto_window(menu->layer, window);
    menu_layer_set_selected_index(menu->layer, menu->selected_index, MenuRowAlignCenter, false);
    layer_add_child(window_layer, menu_layer_get_layer(menu->layer));
}

static void s_window_unload(Window *window) {
    // do nothing
}

static MenuIndex s_menu_get_index_from_pattern(CPattern pattern) {
    MenuIndex indexs[MAX_CPATTERN] = {
        (MenuIndex){0, 0},
        (MenuIndex){0, 0},
        (MenuIndex){1, 0},
        (MenuIndex){1, 1},
        (MenuIndex){1, 2}
    };
    return indexs[(int)pattern];
}
