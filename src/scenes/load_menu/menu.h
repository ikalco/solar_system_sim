#ifndef LOAD_MENU_MENU_H
#define LOAD_MENU_MENU_H

#include "menu/menu.h"
#include "scenes/scene.h"
#include <SDL2/SDL.h>

typedef struct {
	SceneManager *manager;
	SDL_Window *window;
	MenuRoot *root;
	int num_saves;
} Data;

#define ROOT_LIST 0
#define TITLE_TEXT 1
#define SAVE_LIST 2
#define SAVE_BUTTONS 3

MenuRoot *init_menu_root(SDL_Window *window, Data *data);

#endif
