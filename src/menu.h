#pragma once
    
#include <pebble.h>

typedef struct menu Menu;

Menu *menu_create(void);
void menu_destroy(Menu *menu);
