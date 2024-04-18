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
	MenuRoot *root;
	PhysicalBody *selected_body;
	MenuNode *selected_body_node;
	MenuNode *selected_editor;
	char *filename;
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

// BODIES_BUTTONS is the 0th button, so this would be after all of them
#define BODIES_EDITOR_START BODIES_BUTTONS + NUM_BODIES_BUTTONS
#define BODIES_EDITOR_TITLE BODIES_EDITOR_START

// mass editor
#define BODIES_EDITOR_MASS BODIES_EDITOR_START + 1
#define BODIES_EDITOR_MASS_TEXT BODIES_EDITOR_START + 2
#define BODIES_EDITOR_MASS_DECIMAL_EDIT BODIES_EDITOR_START + 3
#define BODIES_EDITOR_MASS_EXPONENT_EDIT BODIES_EDITOR_START + 4

// x pos
#define BODIES_EDITOR_X BODIES_EDITOR_START + 5
#define BODIES_EDITOR_X_TEXT BODIES_EDITOR_START + 6
#define BODIES_EDITOR_X_DECIMAL_EDIT BODIES_EDITOR_START + 7
#define BODIES_EDITOR_X_EXPONENT_EDIT BODIES_EDITOR_START + 8

// y pos
#define BODIES_EDITOR_Y BODIES_EDITOR_START + 9
#define BODIES_EDITOR_Y_TEXT BODIES_EDITOR_START + 10
#define BODIES_EDITOR_Y_DECIMAL_EDIT BODIES_EDITOR_START + 11
#define BODIES_EDITOR_Y_EXPONENT_EDIT BODIES_EDITOR_START + 12

// x vel
#define BODIES_EDITOR_X_VEL BODIES_EDITOR_START + 13
#define BODIES_EDITOR_X_VEL_TEXT BODIES_EDITOR_START + 14
#define BODIES_EDITOR_X_VEL_DECIMAL_EDIT BODIES_EDITOR_START + 15
#define BODIES_EDITOR_X_VEL_EXPONENT_EDIT BODIES_EDITOR_START + 16

// y vel
#define BODIES_EDITOR_Y_VEL BODIES_EDITOR_START + 17
#define BODIES_EDITOR_Y_VEL_TEXT BODIES_EDITOR_START + 18
#define BODIES_EDITOR_Y_VEL_DECIMAL_EDIT BODIES_EDITOR_START + 19
#define BODIES_EDITOR_Y_VEL_EXPONENT_EDIT BODIES_EDITOR_START + 20

MenuRoot *init_solar_system_menu_root(SDL_Window *window, Data *data);

#endif
