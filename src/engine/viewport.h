#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "utils/util.h"
#include <SDL2/SDL.h>

typedef struct vp {
	VectorD position;
	VectorD size;
	VectorD offset;
	double scale;
	double conversion;
} Viewport;

Viewport *init_viewport(VectorD offset, VectorD size, double scale);
void draw_viewport_grid(SDL_Renderer *renderer, Viewport *viewport);
void handle_viewport_nav(Viewport *viewport, SDL_Event *event);

#endif
