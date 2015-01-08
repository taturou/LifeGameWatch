#pragma once

#include <cells.h>

typedef struct cell_font {
    CSize size;
    const uint8_t *data;
} CFont;

const CFont font_number[10];
const CFont font_pattern_glider_lt;       // glider for left-top
const CFont font_pattern_glider_rt;       // glider for right-top
const CFont font_pattern_glider_lb;       // glider for left-bottom
const CFont font_pattern_glider_rb;       // glider for right-bottom
const CFont font_pattern_spaceship_lw;    // Lightweight spaceship
const CFont font_pattern_spaceship_mw;    // Middleweight spaceship
const CFont font_pattern_spaceship_hw;    // Heavyweight spaceship
const CFont font_pattern_pentomino;
