#ifndef INCLUDES_H
#define INCLUDES_H

#include <SDL2/SDL.h>

#include "options.h"
#include "scenes/scene.h"

#include "scenes/load_menu/scene.h"
#include "scenes/main_menu/scene.h"

void init_SDL();
void cleanup_SDL();
void handle_input_SDL(SDL_Event *event);

#endif
