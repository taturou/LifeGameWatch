#pragma once
  
#include <pebble.h>
#include "cells.h"

#define CELL_SIZE_MIN        (2)
#define CELL_SIZE_MAX        (8)
    
typedef struct field_settings {
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
        DRAW_GRID_RANDOM = 0,
        DRAW_GRID_TRUE,
        DRAW_GRID_FALSE
    } is_draw_grid;
} FieldSettings;

#define DEFAULT_CELL_SIZE    (CELL_SIZE_6)
#define DEFAULT_IS_DRAW_GRID (DRAW_GRID_TRUE)

typedef struct field Field;

Field *field_create(GRect window_frame);
void field_destroy(Field *field);
bool field_reset(Field *field, FieldSettings *settings);
void field_mark_dirty(Field *field);
Layer *field_get_layer(const Field *field);
void field_set_pattern(Field *field, CPattern pattern);
bool field_evolution(Field *field);
