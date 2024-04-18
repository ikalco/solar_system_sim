#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

#include "includes.h"

SDL_Window *window;
SDL_Renderer *renderer;
SceneManager *manager;

int main() {
	init_SDL();
	atexit(cleanup_SDL);

	// create menu scene
	Scene *menu_scene = create_scene(init_main_menu,
									 cleanup_main_menu,
									 handle_input_main_menu,
									 draw_main_menu,
									 NULL);

	Scene *load_scene = create_scene(init_load_menu,
									 cleanup_load_menu,
									 handle_input_load_menu,
									 draw_load_menu,
									 NULL);

	// make sure order of scenes stay the same, look at scenes/scene.h
	// create scene manager, add menu_scene, and select it
	manager = create_scene_manager(menu_scene);
	add_scene_manager(manager, load_scene);

	select_scene_manager(manager, window, 0);

	Scene *active_scene;
	SDL_Event event;

	while (1) {
		// get active scene
		active_scene = manager->scenes[manager->active_index];

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				exit(0);
			}

			// handle input for scene
			active_scene->handle_input(active_scene->data, &event);
		}

		// draw scene to screen
		active_scene->draw(active_scene->data, renderer);

		// draw to screen and wait amount of time for desired fps
		SDL_RenderPresent(renderer);
		SDL_Delay(1000 / WINDOW_FPS);
	}

	return 0;
}

void init_SDL() {
	// init sdl2
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL couldn't be initialized.\n");
		exit(1);
	}

	// init sdl2_ttf
	if (TTF_Init() < 0) {
		printf("SDL_ttf couldn't be initialized.\n");
		exit(1);
	}

	// create window
	window = SDL_CreateWindow(WINDOW_TITLE,
							  SDL_WINDOWPOS_UNDEFINED,
							  SDL_WINDOWPOS_UNDEFINED,
							  WINDOW_WIDTH,
							  WINDOW_HEIGHT,
							  SDL_WINDOW_SHOWN);

	// if window is undefined print error and exit
	if (window == NULL) {
		printf("Couldn't create window: %s\n", SDL_GetError());
		exit(1);
	}

	// create renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	// if renderer is undefined print error and exit
	if (renderer == NULL) {
		printf("Couldn't create renderer: %s\n", SDL_GetError());
		exit(1);
	}
}

void cleanup_SDL() {
	destroy_scene_manager(manager);

	SDL_DestroyRenderer(renderer);

	SDL_DestroyWindow(window);

	// clean up sdl2 and sdl2_ttf
	TTF_Quit();
	SDL_Quit();
}
