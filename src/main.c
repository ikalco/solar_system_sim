#include <stdio.h>
#include "defines.h"
#include "bodies.h"
#include "viewport.h"

SDL_Window *window;
SDL_Renderer *renderer;

Viewport* viewport;
List* bodies;

int main() {
	initSDL();
	atexit(cleanup);

	bodies = init_bodies_list();
	viewport = init_viewport();

	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;
	double delta_time = 0;

	while (1) {
		handle_input();

		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();

		delta_time = (NOW-LAST)*1000.0 / (double)SDL_GetPerformanceFrequency();

		update_bodies(bodies, delta_time);

		// clear screen with black
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		draw_viewport_grid(renderer, viewport);
		draw_bodies(renderer, viewport, bodies);

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
			case SDL_KEYUP:
				update_bodies(bodies, 1);
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
		free_list(bodies);
	}

	if (viewport != NULL) {
		free(viewport);
	}

	SDL_DestroyRenderer(renderer);

	SDL_DestroyWindow(window);

	// clean up sdl
	SDL_Quit();
}
