#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "defines.h"

typedef struct vp {
	VectorD offset;
	double cell_size;
} Viewport;

void init_viewport();
void draw_viewport_grid();

#endif
