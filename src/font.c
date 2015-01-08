#include "font.h"

const uint8_t font_number_data[10][7 * 3] = {
    // 0
    {
        1,1,1,
        1,0,1,
        1,0,1,
        1,0,1,
        1,0,1,
        1,0,1,
        1,1,1,
    },
    // 1
    {
        1,1,0,
        0,1,0,
        0,1,0,
        0,1,0,
        0,1,0,
        0,1,0,
        1,1,1,
    },
    // 2
    {
        1,1,1,
        0,0,1,
        0,0,1,
        1,1,1,
        1,0,0,
        1,0,0,
        1,1,1,
    },
    // 3
    {
        1,1,1,
        0,0,1,
        0,0,1,
        1,1,1,
        0,0,1,
        0,0,1,
        1,1,1,
    },
    // 4
    {
        1,0,1,
        1,0,1,
        1,0,1,
        1,1,1,
        0,0,1,
        0,0,1,
        0,0,1,
    },
    // 5
    {
        1,1,1,
        1,0,0,
        1,0,0,
        1,1,1,
        0,0,1,
        0,0,1,
        1,1,1,
    },
    // 6
    {
        1,1,1,
        1,0,0,
        1,0,0,
        1,1,1,
        1,0,1,
        1,0,1,
        1,1,1,
    },
    // 7
    {
        1,1,1,
        0,0,1,
        0,0,1,
        0,1,0,
        0,1,0,
        0,1,0,
        0,1,0,
    },
    // 8
    {
        1,1,1,
        1,0,1,
        1,0,1,
        1,1,1,
        1,0,1,
        1,0,1,
        1,1,1,
    },
    // 9
    {
        1,1,1,
        1,0,1,
        1,0,1,
        1,1,1,
        0,0,1,
        0,0,1,
        1,1,1,
    }
};

const CFont font_number[10] = {
    {{7, 3}, font_number_data[0]},
    {{7, 3}, font_number_data[1]},
    {{7, 3}, font_number_data[2]},
    {{7, 3}, font_number_data[3]},
    {{7, 3}, font_number_data[4]},
    {{7, 3}, font_number_data[5]},
    {{7, 3}, font_number_data[6]},
    {{7, 3}, font_number_data[7]},
    {{7, 3}, font_number_data[8]},
    {{7, 3}, font_number_data[9]}
};

const uint8_t font_pattern_glider_lt_data[3 * 3] = {
    0,1,0,
    0,0,1,
    1,1,1
};

const CFont font_pattern_glider_lt = {
    {3, 3}, font_pattern_glider_lt_data
};

const uint8_t font_pattern_glider_rt_data[3 * 3] = {
    0,1,0,
    1,0,0,
    1,1,1
};

const CFont font_pattern_glider_rt = {
    {3, 3}, font_pattern_glider_rt_data
};

const uint8_t font_pattern_glider_lb_data[3 * 3] = {
    1,1,1,
    0,0,1,
    0,1,0
};

const CFont font_pattern_glider_lb = {
    {3, 3}, font_pattern_glider_lb_data
};

const uint8_t font_pattern_glider_rb_data[3 * 3] = {
    1,1,1,
    1,0,0,
    0,1,0
};

const CFont font_pattern_glider_rb = {
    {3, 3}, font_pattern_glider_rb_data
};

const uint8_t font_pattern_spaceship_lw_data[4 * 5] = {
    1,0,0,1,0,
    0,0,0,0,1,
    1,0,0,0,1,
    0,1,1,1,1
};

const CFont font_pattern_spaceship_lw = {
    {4, 5}, font_pattern_spaceship_lw_data
};

const uint8_t font_pattern_spaceship_mw_data[5 * 6] = {
    0,0,1,0,0,0,
    1,0,0,0,1,0,
    0,0,0,0,0,1,
    1,0,0,0,0,1,
    0,1,1,1,1,1
};

const CFont font_pattern_spaceship_mw = {
    {5, 6}, font_pattern_spaceship_mw_data
};

const uint8_t font_pattern_spaceship_hw_data[5 * 7] = {
    0,0,1,1,0,0,0,
    1,0,0,0,0,1,0,
    0,0,0,0,0,0,1,
    1,0,0,0,0,0,1,
    0,1,1,1,1,1,1
};

const CFont font_pattern_spaceship_hw = {
    {5, 7}, font_pattern_spaceship_hw_data
};

const uint8_t font_pattern_pentomino_data[3 * 3] = {
    0,1,1,
    1,1,0,
    0,1,0
};

const CFont font_pattern_pentomino = {
    {3, 3}, font_pattern_pentomino_data
};
