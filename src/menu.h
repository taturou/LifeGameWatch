#pragma once
    
#include <pebble.h>

typedef enum pattern {
    None,
    Clock,
    Glider,
    LWSS
} Pattern;
    
typedef struct menu Menu;

typedef void (*MenuSelectCallback)(Pattern pattern);

Menu *menu_create(MenuSelectCallback callback);
void menu_destroy(Menu *menu);
