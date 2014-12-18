#pragma once
  
#include <pebble.h>

typedef struct field Field;

Field *field_create(GRect window_frame);
void field_destroy(Field *field);
void field_mark_dirty(Field *field);
Layer *field_get_layer(const Field *field);
void field_clear(Field *field);
void field_set_time(Field *field, time_t tim);
void field_evolution(Field *field);
