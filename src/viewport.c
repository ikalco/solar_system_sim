#include "viewport.h"
#include "options.h"

Viewport* init_viewport() {
	Viewport* viewport = malloc(sizeof(Viewport));

	int min_size;

	if (WINDOW_WIDTH < WINDOW_HEIGHT) {
		min_size = WINDOW_WIDTH;
	} else {
		min_size = WINDOW_HEIGHT;
	}

	viewport->cell_size = (double) min_size / VIEWPORT_SIZE;
	viewport->conversion = viewport->cell_size / VIEWPORT_UNIT;

	viewport->offset.x = (double) WINDOW_WIDTH / 2;
	viewport->offset.y = (double) WINDOW_HEIGHT / 2;

	return viewport;
}

void draw_viewport_grid(SDL_Renderer* renderer, Viewport* viewport) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	// vertical lines
	for(int i = 1; i < WINDOW_WIDTH / viewport->cell_size; i++) {
		SDL_RenderDrawLine(renderer, i * viewport->cell_size, 0, i * viewport->cell_size, WINDOW_HEIGHT);
	}

	// horizontal lines
	for(int i = 1; i < WINDOW_HEIGHT / viewport->cell_size; i++) {
		SDL_RenderDrawLine(renderer, 0, i * viewport->cell_size, WINDOW_WIDTH, i * viewport->cell_size);
	}
}
