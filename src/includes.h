#ifndef INCLUDES_H
#define INCLUDES_H

#include <SDL2/SDL.h>

#include "options.h"

#include "utils/linked_list.h"
#include "utils/util.h"

#include "engine/bodies.h"
#include "engine/physical.h"
#include "engine/save.h"
#include "engine/viewport.h"

#include "menu/menu.h"
#include "scene/scene.h"

void init_SDL();
void cleanup_SDL();
void handle_input_SDL(SDL_Event* event);

#endif
