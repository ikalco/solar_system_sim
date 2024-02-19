#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdint.h>

typedef struct vd {
	double x;
	double y;
} VectorD;

typedef struct color {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} Color;

#endif
