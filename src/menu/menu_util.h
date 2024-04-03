#ifndef MENU_UTIL_H
#define MENU_UTIL_H

#include "menu.h"

int find_mouse_menu_node(MenuNode *node, int mouse_x, int mouse_y);
int find_mouse_menu_root(MenuRoot *root, int mouse_x, int mouse_y);

MenuNode *find_menu_node_id(MenuNode *node, int id);

int menu_has_clicked(MenuRoot *root, SDL_Event *event);

SDL_Rect get_menu_offset(MenuNode *node);

#endif
