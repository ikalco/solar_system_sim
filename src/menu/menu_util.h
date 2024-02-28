#ifndef MENU_UTIL_H
#define MENU_UTIL_H

#include "menu.h"

int find_mouse_menu_node(MenuNode *node, int mouse_x, int mouse_y);
int find_mouse_menu_root(MenuRoot *root, int mouse_x, int mouse_y);

#endif
