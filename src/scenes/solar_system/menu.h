#ifndef SOLAR_SYSTEM_MENU_H
#define SOLAR_SYSTEM_MENU_H

#include "engine/bodies.h"
#include "engine/viewport.h"
#include "menu/menu.h"
#include "scenes/scene.h"
#include <SDL2/SDL.h>

typedef struct {
	SceneManager *manager;
	SDL_Window *window;
	List *bodies;
	Viewport *viewport;
	char *name;
	MenuRoot *root;
	PhysicalBody *selected_body;
	int selected_body_id;
} Data;

#define ROOT_LIST 0
#define TITLE_TEXT 1
#define LINE_BREAK 2
#define BODIES_LIST 3
#define BODIES_BUTTONS 4

MenuRoot *init_solar_system_menu_root(SDL_Window *window, Data *data);

#endif
