#pragma once
    
#include <pebble.h>
#include "cells.h"

typedef struct menu Menu;

typedef void (*MenuSelectCallback)(CPattern pattern);

Menu *menu_create(CPattern now_pattern, MenuSelectCallback callback);
void menu_destroy(Menu *menu);
