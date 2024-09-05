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

	// make sure order of scenes stay the same, look at scenes/scene.h
	// create scene manager, add menu_scene, and select it
	manager = create_scene_manager();

	add_scene_manager(manager,
					  (Scene){.init = init_main_menu,
							  .cleanup = cleanup_main_menu,
							  .handle_input = handle_input_main_menu,
							  .draw = draw_main_menu});
	add_scene_manager(manager,
					  (Scene){.init = init_load_menu,
							  .cleanup = cleanup_load_menu,
							  .handle_input = handle_input_load_menu,
							  .draw = draw_load_menu});

	select_scene_manager(manager, window, SCENE_MAIN_MENU_ID);

	Scene *active_scene;
	SDL_Event event;

	while (1) {
		// get active scene
		active_scene = manager->scenes[manager->active_index];

		// draw scene to screen
		active_scene->draw(active_scene->data, renderer);

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				exit(0);
			}

			// handle input for scene
			active_scene->handle_input(active_scene->data, &event);
		}

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
