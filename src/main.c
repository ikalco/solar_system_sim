#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include <SDL2/SDL.h>

SDL_Window *window;
SDL_Renderer *renderer;

void initSDL() {
	// init sdl2
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL couldn't be initialized.\n");
		exit(1);
	} else {
		printf("SDL video system is ready to go.\n");
	}

	// create window
	window = SDL_CreateWindow(
		"An SDL2 window",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,
		480,
		SDL_WINDOW_SHOWN
	);

	// if window is undefined print error and exit
	if (window == NULL) {
		printf("Couldn't create window: %s\n", SDL_GetError());
		exit(1);
	}

	// create renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void cleanup() {
	// destroy the window
	SDL_DestroyWindow(window);

	// clean up
	SDL_Quit();
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

int main(int argc, char* argv[]) {
	initSDL();
	atexit(cleanup);

	while (1) {
		// clear screen
		SDL_RenderClear(renderer);

		// draw a line
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderDrawLine(renderer, 0, 0, 640, 480);

		// draw to screen and wait 16ms (62.5 fps)
		SDL_RenderPresent(renderer);
		SDL_Delay(16);
	}

	return 0;
}
