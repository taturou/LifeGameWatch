#include <pebble.h>
#include "menu.h"

#define NUM_MENU_SECTIONS       (3)
#define NUM_MENU_SECTION1_ROWS  (1)
#define NUM_MENU_SECTION2_ROWS  (3)
#define NUM_MENU_SECTION3_ROWS  (1)

typedef struct menu {
    Window *window;
    MenuLayer *layer;
    GBitmap *pattern_icons[MAX_CPATTERN];
    GBitmap *setting_icon;
    MenuIndex selected_index;
    MenuSelectCallback callback;
} Menu;

typedef struct menu_field_settings {
    enum {
        CELL_SIZE_RANDOM = 0,
        CELL_SIZE_2 = CELL_SIZE_MIN,
        CELL_SIZE_3,
        CELL_SIZE_4,
        CELL_SIZE_5,
        CELL_SIZE_6,
        CELL_SIZE_7,
        CELL_SIZE_8,
    } cell_size;
    enum {
        DRAW_GRID_TRUE,
        DRAW_GRID_FALSE,
        DRAW_GRID_RANDOM
    } is_draw_grid;
} MenuFieldSettings;

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
            menu->pattern_icons[CP_Clock] = gbitmap_create_with_resource(RESOURCE_ID_MENU_ICON_CLOCK);
            menu->pattern_icons[CP_Glider] = gbitmap_create_with_resource(RESOURCE_ID_MENU_ICON_GLIDER);
            menu->pattern_icons[CP_LWSaceship] = gbitmap_create_with_resource(RESOURCE_ID_MENU_ICON_LWSS);
            menu->pattern_icons[CP_RRntomino] = gbitmap_create_with_resource(RESOURCE_ID_MENU_ICON_RPENT);
            menu->setting_icon = gbitmap_create_with_resource(RESOURCE_ID_MENU_ICON_SETTING);

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

        gbitmap_destroy(menu->pattern_icons[CP_Clock]);
        gbitmap_destroy(menu->pattern_icons[CP_Glider]);
        gbitmap_destroy(menu->pattern_icons[CP_LWSaceship]);
        gbitmap_destroy(menu->pattern_icons[CP_RRntomino]);
        gbitmap_destroy(menu->setting_icon);
    }
    free(menu);
}

static uint16_t s_menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
    return NUM_MENU_SECTIONS;
}

static uint16_t s_menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
    const uint16_t num_rows[NUM_MENU_SECTIONS] = {
        NUM_MENU_SECTION1_ROWS,
        NUM_MENU_SECTION2_ROWS,
        NUM_MENU_SECTION3_ROWS
    };
    return num_rows[section_index];
}

static int16_t s_menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
    // This is a define provided in pebble.h that you may use for the default height
    return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static void s_menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
    const char *titles[NUM_MENU_SECTIONS] = {
        "Special pattern",
        "Popular pattern",
        "Other"
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
    const struct basic_cell cells1[NUM_MENU_SECTION1_ROWS] = {
        {"Clock", "The present time", menu->pattern_icons[CP_Clock]}
    };
    const struct basic_cell cells2[NUM_MENU_SECTION2_ROWS] = {
        {"Glider", "Most popular glider", menu->pattern_icons[CP_Glider]},
        {"Spaceship", "Lightweight spaceship", menu->pattern_icons[CP_LWSaceship]},
        {"R-pentomino", "Not stabilize", menu->pattern_icons[CP_RRntomino]}
    };
    const struct basic_cell cells3[NUM_MENU_SECTION3_ROWS] = {
        {"Settings", "for size, display", menu->setting_icon}
    };
    const struct basic_cell *cells[NUM_MENU_SECTIONS] = {
        cells1,
        cells2,
        cells3
    };
    const struct basic_cell *cell = &cells[cell_index->section][cell_index->row];
    menu_cell_basic_draw(ctx, cell_layer, cell->title, cell->sub_title, cell->icon);
}

static void s_menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
    Menu *menu = (Menu*)data;
    
    if (cell_index->section != 2) {
        // pattern
        const CPattern patterns1[NUM_MENU_SECTION1_ROWS] = {
            CP_Clock
        };        
        const CPattern patterns2[NUM_MENU_SECTION2_ROWS] = {
            CP_Glider,
            CP_LWSaceship,
            CP_RRntomino 
        };
        const CPattern *patterns[NUM_MENU_SECTIONS] = {
            patterns1,
            patterns2
        };
        const CPattern *pattern = &patterns[cell_index->section][cell_index->row];

        // settings
        const MenuFieldSettings msettings1[NUM_MENU_SECTION1_ROWS] = {
            {CELL_SIZE_RANDOM, DRAW_GRID_RANDOM}
        };
        const MenuFieldSettings msettings2[NUM_MENU_SECTION2_ROWS] = {
            {CELL_SIZE_RANDOM, DRAW_GRID_RANDOM},
            {CELL_SIZE_RANDOM, DRAW_GRID_RANDOM},
            {CELL_SIZE_RANDOM, DRAW_GRID_RANDOM}
        };
        const MenuFieldSettings *msettings[NUM_MENU_SECTIONS] = {
            msettings1,
            msettings2
        };
        MenuFieldSettings msetting = msettings[cell_index->section][cell_index->row];
        FieldSettings setting;
        
        switch (msetting.cell_size) {
        case CELL_SIZE_RANDOM:
            setting.cell_size = (rand() % (CELL_SIZE_MAX - CELL_SIZE_MIN)) + CELL_SIZE_MIN;
            break;
        default:
            setting.cell_size = (int)msetting.cell_size;
            break;
        }
        
        switch (msetting.is_draw_grid) {
        case DRAW_GRID_TRUE:
            setting.is_draw_grid = true;
            break;
        case DRAW_GRID_FALSE:
            setting.is_draw_grid = false;
            break;
        case DRAW_GRID_RANDOM:
            setting.is_draw_grid = rand() % 2 == 0 ? true : false;
            break;
        }

        (*menu->callback)(*pattern, setting);

        menu_destroy(menu);
    } else { // cell_index->section == 2
        
    }
}

static void s_window_load(Window *window) {
    Menu *menu = window_get_user_data(window);
    
    Layer *window_layer = window_get_root_layer(window);

    menu->layer = menu_layer_create(layer_get_frame(window_layer));
    menu_layer_set_callbacks(menu->layer, (void*)menu, (MenuLayerCallbacks){
        .get_num_sections = s_menu_get_num_sections_callback,
        .get_num_rows = s_menu_get_num_rows_callback,
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
    Menu *menu = window_get_user_data(window);

    menu_layer_destroy(menu->layer);
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
