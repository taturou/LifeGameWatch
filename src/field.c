#include <pebble.h>
#include "field.h"
#include "cells.h"

#define CELL_SIZE   (8)  // width is same as height

typedef struct field {
    Layer *layer;
    Cells *cells;
} Field;

static void s_layer_update_callback(Layer *layer, GContext *ctx);

Field *field_create(GRect window_frame) {
    Field *field = NULL;

    Layer *layer = layer_create_with_data(window_frame, sizeof(Field));
    if (layer != NULL) {
        // init layer
        GRect frame;
        frame.origin.x = CELL_SIZE - 1;
        frame.origin.y = CELL_SIZE - 1;
        frame.size.w = window_frame.size.w - ((CELL_SIZE * 2) - 1);
        frame.size.h = window_frame.size.h - ((CELL_SIZE * 2) - 1);

        layer_set_frame(layer, frame);
        layer_set_update_proc(layer, s_layer_update_callback);

        // init field
        field = (Field*)layer_get_data(layer);
        memset(field, 0, sizeof(Field));

        field->layer = layer;
        field->cells = cells_create((CSize){frame.size.h / CELL_SIZE, frame.size.w / CELL_SIZE});
        if (field->cells == NULL) {
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

void field_mark_dirty(Field *field) {
    layer_mark_dirty(field->layer);
}

Layer *field_get_layer(const Field *field) {
    return field->layer;
}

void field_clear(Field *field) {
    cells_clear(field->cells);
    field_mark_dirty(field);
}

void field_set_time(Field *field, time_t tim) {
    cells_set_time(field->cells, tim);
    field_mark_dirty(field);
}

void field_evolution(Field *field) {
    cells_evolution(field->cells);
    field_mark_dirty(field);
}

static void s_draw_grid(GContext *ctx, Field *field) {
    CSize size = cells_get_size(field->cells);

    for (int row = 0; row <= size.row; row++) {
        graphics_draw_line(ctx, (GPoint){0, row * CELL_SIZE}, (GPoint){size.column * CELL_SIZE, row * CELL_SIZE});
    }
    for (int col = 0; col <= size.column; col++) {
        graphics_draw_line(ctx, (GPoint){col * CELL_SIZE, 0}, (GPoint){col * CELL_SIZE, size.row * CELL_SIZE});
    }   
}

static void s_draw_cells(GContext *ctx, Field *field) {
    CSize size = cells_get_size(field->cells);

    GRect rect;
    rect.size.w = CELL_SIZE;
    rect.size.h = CELL_SIZE;

    for (int row = 0; row < size.row; row++) {
        for (int col = 0; col < size.column; col++) {
            if (cells_is_alive(field->cells, row, col) == true) {
                rect.origin.x = col * CELL_SIZE;
                rect.origin.y = row * CELL_SIZE;
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