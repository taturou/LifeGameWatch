#pragma once
  
#include <pebble.h>
#include "cells.h"

typedef struct field Field;

Field *field_create(GRect window_frame);
void field_destroy(Field *field);
bool field_reset_cell_size(Field *field, int cell_size);
void field_draw_grid(Field *field, bool is_draw);
void field_mark_dirty(Field *field);
Layer *field_get_layer(const Field *field);
void field_set_pattern(Field *field, CPattern pattern);
bool field_evolution(Field *field);
