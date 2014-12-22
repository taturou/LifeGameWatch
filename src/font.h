#pragma once

#include <cells.h>

typedef struct cell_font {
    CSize size;
    const uint8_t *data;
} CFont;

const CFont font_number[10];
const CFont font_pattern_glider;
const CFont font_pattern_lwspaceship;
const CFont font_pattern_pentomino;
