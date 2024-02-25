#ifndef UTIL_H
#define UTIL_H

#include "linked_list.h"
#include <stdint.h>

typedef struct vd {
	double x;
	double y;
} VectorD;

// type punning with SDL_Color, so don't change size/order of Color
typedef struct color {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t alpha;
} Color;

char* create_string(const char* str);

#endif
