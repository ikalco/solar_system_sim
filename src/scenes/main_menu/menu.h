#ifndef MAIN_MENU_MENU_H
#define MAIN_MENU_MENU_H

#include "menu/menu.h"
#include "scenes/scene.h"
#include <SDL2/SDL.h>

/*
MenuRoot (MenuList) id=0
	MenuText		id=1
	MenuList		id=2
		MenuButton	id=3
		MenuButton	id=4
		MenuButton	id=5
	MenuButton		id=6
*/

#define ROOT_LIST 0
#define MENUTEXT 1
#define SUB_LIST 2
#define CREATE_BUTTON 3
#define LOAD_BUTTON 4
#define SETTINGS_BUTTON 5
#define EXIT_BUTTON 6

typedef struct {
	SceneManager *manager;
	SDL_Window *window;
	MenuRoot *root;
	int clicked_id;
} Data;

MenuRoot *init_main_menu_root(SDL_Window *window);

#endif
