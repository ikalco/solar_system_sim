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
	MenuNode *selected_body_node;
} Data;

#define DEFAULT_BUTTON_COLOR                                                   \
	(Color) { 40, 40, 40, 255 }
#define SELECTED_BUTTON_COLOR                                                  \
	(Color) { 50, 50, 50, 255 }

#define ROOT_LIST 0
#define TITLE_TEXT 1
#define LINE_BREAK 2
#define BODIES_LIST 3
#define BODIES_BUTTONS 4
#define NUM_BODIES_BUTTONS 5

MenuRoot *init_solar_system_menu_root(SDL_Window *window, Data *data);

#endif
