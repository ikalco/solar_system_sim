#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "util.h"
#include <SDL2/SDL.h>

typedef struct vp {
	VectorD offset;
	double cell_size;
	double conversion;
} Viewport;

Viewport* init_viewport();
void draw_viewport_grid(SDL_Renderer* renderer, Viewport* viewport);

#endif
