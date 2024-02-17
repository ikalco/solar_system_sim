#include "viewport.h"

extern SDL_Renderer* renderer;

Viewport* viewport;

void init_viewport() {
	viewport = malloc(sizeof(Viewport));

	int min_size;

	if (WINDOW_WIDTH < WINDOW_HEIGHT) {
		min_size = WINDOW_WIDTH;
	} else {
		min_size = WINDOW_HEIGHT;
	}

	viewport->cell_size = (double) min_size / VIEWPORT_SIZE;

	viewport->offset.x = (double) WINDOW_WIDTH / 2;
	viewport->offset.y = (double) WINDOW_HEIGHT / 2;
}

void draw_viewport_grid() {
	int min_size;

	if (WINDOW_WIDTH < WINDOW_HEIGHT) {
		min_size = WINDOW_WIDTH;
	} else {
		min_size = WINDOW_HEIGHT;
	}

	double cell_size = (double) min_size / VIEWPORT_SIZE;

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	// vertical lines
	for(int i = 1; i < WINDOW_WIDTH / cell_size; i++) {
		SDL_RenderDrawLine(renderer, i * cell_size, 0, i * cell_size, WINDOW_HEIGHT);
	}

	// horizontal lines
	for(int i = 1; i < WINDOW_HEIGHT / cell_size; i++) {
		SDL_RenderDrawLine(renderer, 0, i * cell_size, WINDOW_WIDTH, i * cell_size);
	}
}
