#pragma once
  
#include <pebble.h>
#include "cells.h"

typedef struct field Field;

Field *field_create(GRect window_frame);
void field_destroy(Field *field);
void field_mark_dirty(Field *field);
Layer *field_get_layer(const Field *field);
void field_set_pattern(Field *field, CPattern pattern);
void field_evolution(Field *field);
