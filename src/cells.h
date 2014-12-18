#pragma once

#include <pebble.h>

typedef struct cells_size {
    uint16_t row;
    uint16_t column;
} CSize;

typedef struct cells Cells;

Cells *cells_create(CSize size);
void cells_destroy(Cells *cells);
CSize cells_get_size(const Cells *cells);
bool cells_is_alive(const Cells *cells, uint16_t row, uint16_t column);
void cells_clear(Cells *cells);
void cells_set_time(Cells *cells, time_t tim);
void cells_evolution(Cells *cells);
