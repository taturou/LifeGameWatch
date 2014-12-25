#pragma once
  
#include <pebble.h>
#include "cells.h"

typedef struct field_settings {
    int cell_size;
    bool is_draw_grid;
} FieldSettings;

#define CELL_SIZE_MIN        (2)
#define CELL_SIZE_MAX        (8)
    
#define DEFAULT_CELL_SIZE    (6)
#define DEFAULT_IS_DRAW_GRID (true)

typedef struct field Field;

Field *field_create(GRect window_frame);
void field_destroy(Field *field);
bool field_reset(Field *field, FieldSettings *settings);
//bool field_reset_cell_size(Field *field, int cell_size);
//void field_draw_grid(Field *field, bool is_draw);
void field_mark_dirty(Field *field);
Layer *field_get_layer(const Field *field);
void field_set_pattern(Field *field, CPattern pattern);
bool field_evolution(Field *field);
