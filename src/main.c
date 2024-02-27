#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

#include "includes.h"

SDL_Window *window;
SDL_Renderer *renderer;
SceneManager *manager;

// Viewport *viewport = NULL;
// List *bodies = NULL;
// MenuRoot *menu_root = NULL;

int main() {
	Scene *menu_scene = create_scene(
		init_menu, cleanup_menu, handle_input_menu, draw_menu, NULL
	);

	manager = create_scene_manager(menu_scene);
	manager->scenes[0]->init(manager->scenes[0]->data, window);
	select_scene_manager(manager, window, 0);

	init_SDL();
	atexit(cleanup_SDL);

	// bodies = read_save_file(DEFAULT_SAVE_FILE);
	// viewport = init_viewport();
	// menu_root = init_main_menu(window);

	Scene *active_scene;
	SDL_Event event;

	while (1) {
		active_scene = manager->scenes[manager->active_index];

		while (SDL_PollEvent(&event)) {
			handle_input_SDL(&event);
			active_scene->handle_input(active_scene->data, &event);
		}
		// update_bodies(bodies, TIME_STEP);

		// clear screen with black
		// SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		// SDL_RenderClear(renderer);

		// draw_menu_root(menu_root);
		// draw_viewport_grid(renderer, viewport);
		// draw_bodies(renderer, viewport, bodies);

		active_scene->draw(active_scene->data, renderer);

		// draw to screen and wait amount of time for desired fps
		SDL_RenderPresent(renderer);
		SDL_Delay(1000 / WINDOW_FPS);
	}

	return 0;
}

void handle_input_SDL(SDL_Event *event) {
	if (event->type == SDL_QUIT) {
		exit(0);
	}
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
	window = SDL_CreateWindow(
		WINDOW_TITLE,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_SHOWN
	);

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
	// if (bodies != NULL) {
	// 	free_bodies(bodies);
	// }
	//
	// if (viewport != NULL) {
	// 	free(viewport);
	// }
	//
	// if (menu_root != NULL) {
	// 	free_menu_root(menu_root);
	// }

	destroy_scene_manager(manager);

	SDL_DestroyRenderer(renderer);

	SDL_DestroyWindow(window);

	// clean up sdl2 and sdl2_ttf
	// TTF_Quit();
	SDL_Quit();
}
