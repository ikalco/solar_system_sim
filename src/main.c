#include <stdio.h>
#include <SDL2/SDL.h>

int main(int argc, char* argv[]) {

	// create window data type
	SDL_Window *window;

	// init sdl2
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL couldn't be initialized.\n");
		return 1;
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
		return 1;
	}

	// create renderer
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	// clear screen and draw it
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	// wait 3 sec so you can see the window
	SDL_Delay(3000);
	
	// destroy the window
	SDL_DestroyWindow(window);

	// clean up
	SDL_Quit();

	return 0;
}
