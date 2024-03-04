#include "viewport.h"
#include "options.h"

Viewport *init_viewport(VectorD offset, VectorD size, double scale) {
	Viewport *viewport = malloc(sizeof(Viewport));

	double min_size;

	if (size.x < size.y) {
		min_size = size.x;
	} else {
		min_size = size.y;
	}

	viewport->cell_size = min_size;
	viewport->conversion = min_size / scale;

	viewport->offset.x = (double)size.x / 2 + offset.x;
	viewport->offset.y = (double)size.y / 2 + offset.y;

	return viewport;
}

void draw_viewport_grid(SDL_Renderer *renderer, Viewport *viewport) {
	// SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	//
	// // vertical lines
	// for (int i = 1; i < WINDOW_WIDTH / viewport->cell_size; i++) {
	// 	SDL_RenderDrawLine(
	// 		renderer,
	// 		i * viewport->cell_size,
	// 		0,
	// 		i * viewport->cell_size,
	// 		WINDOW_HEIGHT
	// 	);
	// }
	//
	// // horizontal lines
	// for (int i = 1; i < WINDOW_HEIGHT / viewport->cell_size; i++) {
	// 	SDL_RenderDrawLine(
	// 		renderer,
	// 		0,
	// 		i * viewport->cell_size,
	// 		WINDOW_WIDTH,
	// 		i * viewport->cell_size
	// 	);
	// }
}
