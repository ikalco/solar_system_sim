#ifndef INCLUDES_H
#define INCLUDES_H

#include "menu/main_menu_scene.h"
#include "options.h"

void init_SDL();
void cleanup_SDL();
void handle_input_SDL(SDL_Event *event);

#endif
