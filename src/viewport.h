#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "defines.h"

// viewport defines
#define VIEWPORT_SIZE 2.5
#define VIEWPORT_UNIT AU

typedef struct vp {
	VectorD offset;
	double cell_size;
	double conversion;
} Viewport;

Viewport* init_viewport();
void draw_viewport_grid(SDL_Renderer* renderer, Viewport* viewport);

#endif
