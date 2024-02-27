#ifndef MENU_SCENE_H
#define MENU_SCENE_H

#include "scene/scene.h"
#include <SDL2/SDL.h>

void init_menu(Scene *scene, SDL_Window *window);
void cleanup_menu(void *data);
void handle_input_menu(void *data, SDL_Event *event);
void draw_menu(void *data, SDL_Renderer *renderer);

#endif
