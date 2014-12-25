#include <pebble.h>
#include "field.h"
#include "cells.h"

typedef struct field {
    Layer *layer;
    GRect window_frame;
    int cell_size;
    Cells *cells;
    bool is_draw_grid;
} Field;

static void s_layer_update_callback(Layer *layer, GContext *ctx);
static bool s_setting_cell_size(Field *field, int cell_size);
static void s_setting_is_draw_grid(Field *field, bool is_draw);

Field *field_create(GRect window_frame) {
    Field *field = NULL;

    Layer *layer = layer_create_with_data(window_frame, sizeof(Field));
    if (layer != NULL) {
        field = (Field*)layer_get_data(layer);
        field->layer = layer;
        field->window_frame = window_frame;
        field->cell_size = 0;
        field->cells = NULL;
        field->is_draw_grid = DEFAULT_IS_DRAW_GRID;
        if (s_setting_cell_size(field, DEFAULT_CELL_SIZE) == true) {
            layer_set_update_proc(layer, s_layer_update_callback);
        } else {
            field_destroy(field);
            field = NULL;
        }
    }
    return field; 
}

void field_destroy(Field *field) {
    if (field == NULL) {
        return;
    }
    cells_destroy(field->cells);
    layer_destroy(field->layer);
}

bool field_reset(Field *field, FieldSettings *settings) {
    bool ret;
    int cell_size;
    bool is_draw_grid;

    switch (settings->cell_size) {
    case CELL_SIZE_RANDOM:
        cell_size = (rand() % (CELL_SIZE_MAX - CELL_SIZE_MIN)) + CELL_SIZE_MIN;
        break;
    default:
        cell_size = (int)settings->cell_size;
        break;
    }

    if (cell_size <= 3) {
        is_draw_grid = false;
    } else {
        switch (settings->is_draw_grid) {
        case DRAW_GRID_TRUE:
            is_draw_grid = true;
            break;
        case DRAW_GRID_FALSE:
            is_draw_grid = false;
            break;
        case DRAW_GRID_RANDOM: // fall down
        default:
            is_draw_grid = rand() % 2 == 0 ? true : false;
            break;
        }
    }
    
    ret = s_setting_cell_size(field, cell_size);
    s_setting_is_draw_grid(field, is_draw_grid);
    
    return ret;
}

void field_mark_dirty(Field *field) {
    layer_mark_dirty(field->layer);
}

Layer *field_get_layer(const Field *field) {
    return field->layer;
}

void field_set_pattern(Field *field, CPattern pattern) {
    cells_set_pattern(field->cells, pattern);
    field_mark_dirty(field);
}

bool field_evolution(Field *field) {
    int ret;
    ret = cells_evolution(field->cells);
    field_mark_dirty(field);
    return ret;
}

static void s_draw_grid(GContext *ctx, Field *field) {
    CSize size = cells_get_size(field->cells);

    if (field->is_draw_grid == true) {
        for (int row = 0; row <= size.row; row++) {
            graphics_draw_line(ctx, (GPoint){0, row * field->cell_size}, (GPoint){size.column * field->cell_size, row * field->cell_size});
        }
        for (int col = 0; col <= size.column; col++) {
            graphics_draw_line(ctx, (GPoint){col * field->cell_size, 0}, (GPoint){col * field->cell_size, size.row * field->cell_size});
        }   
    }
}

static void s_draw_cells(GContext *ctx, Field *field) {
    CSize size = cells_get_size(field->cells);

    GRect rect;
    rect.size.w = field->cell_size;
    rect.size.h = field->cell_size;

    for (int row = 0; row < size.row; row++) {
        for (int col = 0; col < size.column; col++) {
            if (cells_is_alive(field->cells, row, col) == true) {
                rect.origin.x = col * field->cell_size;
                rect.origin.y = row * field->cell_size;
                graphics_fill_rect(ctx, rect, 0, GCornerNone);                
            }
        }
    }
}

static void s_layer_update_callback(Layer *layer, GContext *ctx) {
    Field *field = (Field*)layer_get_data(layer);

    graphics_context_set_stroke_color(ctx, GColorWhite);
    graphics_context_set_fill_color(ctx, GColorWhite);

    // draw the grid
    s_draw_grid(ctx, field);

    // draw cells
    s_draw_cells(ctx, field);
}

static bool s_setting_cell_size(Field *field, int cell_size) {
    int ret = false;

    if ((cell_size < CELL_SIZE_MIN) || (CELL_SIZE_MAX < cell_size)) {
        return false;
    }
    
    if (field->cell_size == cell_size) {
        return true;
    }

    // destroy
    cells_destroy(field->cells);
    field->cells = NULL;
    
    // init layer frame
    GRect frame;
    frame.origin.x = cell_size - 1;
    frame.origin.y = cell_size - 1;
    frame.size.w = field->window_frame.size.w - ((cell_size * 2) - 1) + (cell_size & 0x1);
    frame.size.h = field->window_frame.size.h - ((cell_size * 2) - 1) + (cell_size & 0x1);
    layer_set_frame(field->layer, frame);

    // init field
    field->cell_size = cell_size;
    field->cells = cells_create((CSize){frame.size.h / cell_size, frame.size.w / cell_size});
    if (field->cells != NULL) {
        ret = true;
    }
    return ret;
}

static void s_setting_is_draw_grid(Field *field, bool is_draw) {
    field->is_draw_grid = is_draw;
}
