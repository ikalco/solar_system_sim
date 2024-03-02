#ifndef SOLAR_SYSTEM_SCENE_H
#define SOLAR_SYSTEM_SCENE_H

#include "scenes/scene.h"
#include <SDL2/SDL.h>

void init_solar_system(SceneManager *manager, Scene *scene, SDL_Window *window);
void cleanup_solar_system(void *data);
void handle_input_solar_system(void *data, SDL_Event *event);
void draw_solar_system(void *data, SDL_Renderer *renderer);

#endif
