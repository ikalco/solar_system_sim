#ifndef DEFINES_H
#define DEFINES_H

#include <stdint.h>
#include <SDL2/SDL.h>
#include "linked_list.h"

// constants
#define G_CONSTANT 6.67430e-11

// units
#define M 6.68458712e-12		// meters
#define KM 6.68458712e-9		// kilometers	
#define AU 1					// astronomical unit (149597870700 in meters)
#define G 0.001					// grams
#define KG 1					// kilograms
#define S 1						// seconds

// derived units
#define M_S 1					// meters per second
#define KM_S 1000				// kilometers per second

// viewport defines
#define VIEWPORT_SIZE 10 * AU

// window defines
#define WINDOW_TITLE "Solar System Simulator"
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define WINDOW_FPS 60

typedef struct vd {
	double x;
	double y;
} VectorD;

typedef struct color {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} Color;

void initSDL();
void cleanup();
void handle_input();

#endif
