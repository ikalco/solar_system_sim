#ifndef INCLUDES_H
#define INCLUDES_H

#include "options.h"
#include "menu/menu_scene.h"

void init_SDL();
void cleanup_SDL();
void handle_input_SDL(SDL_Event *event);

#endif
