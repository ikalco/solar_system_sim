#ifndef LOAD_MENU_SCENE_H
#define LOAD_MENU_SCENE_H

#include "scenes/scene.h"
#include <SDL2/SDL.h>

void init_load_menu(SceneManager *manager, Scene *scene, SDL_Window *window);
void cleanup_load_menu(void *data);
void handle_input_load_menu(void *data, SDL_Event *event);
void draw_load_menu(void *data, SDL_Renderer *renderer);

#endif
