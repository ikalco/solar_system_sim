#include <stdio.h>
#include <SDL2/SDL.h>

#include "includes.h"

SDL_Window *window;
SDL_Renderer *renderer;

Viewport* viewport = NULL;
List* bodies = NULL;
MenuNode* menu_root = NULL;

int main() {
	initSDL();
	atexit(cleanup);

	bodies = read_save_file(DEFAULT_SAVE_FILE);
	viewport = init_viewport();

	MenuList* menu_list = create_menu_list((VectorD){20, 20}, 200, 300, (Color){20, 20, 20, 255}, 10, 20);
	menu_root = create_menu_root();
	menu_root->node = menu_list;
	menu_root->type = MENU_LIST;

	while (1) {
		handle_input();

		// update_bodies(bodies, TIME_STEP);

		// clear screen with black
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		draw_menu_node(renderer, menu_root);

		// draw_viewport_grid(renderer, viewport);
		// draw_bodies(renderer, viewport, bodies);

		// draw to screen and wait amount of time for desired fps
		SDL_RenderPresent(renderer);
		SDL_Delay(1000/WINDOW_FPS);
	}

	return 0;
}

void handle_input() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				exit(0);
				break;
			default:
				break;
		}
	}
}

void initSDL() {
	// init sdl2
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL couldn't be initialized.\n");
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

void cleanup() {
	if (bodies != NULL) {
		free_bodies(bodies);
	}

	if (viewport != NULL) {
		free(viewport);
	}

	if (menu_root != NULL) {
		free_menu_node(menu_root);
	}

	SDL_DestroyRenderer(renderer);

	SDL_DestroyWindow(window);

	// clean up sdl
	SDL_Quit();
}
