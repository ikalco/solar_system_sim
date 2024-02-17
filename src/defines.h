#ifndef DEFINES_H
#define DEFINES_H

#include <stdint.h>
#include <SDL2/SDL.h>
#include "linked_list.h"

// constants
#define G_CONSTANT 6.67430e-11

// units
#define M 1.0					// meters
#define KM 1000.0				// kilometers	
#define AU 149597870700.0		// astronomical unit (149597870700 in meters)
#define G 0.001					// grams
#define KG 1.0					// kilograms
#define S 1.0					// seconds

// derived units
#define M_S M / S					// meters per second
#define KM_S KM / S				// kilometers per second

// window defines
#define WINDOW_TITLE "Solar System Simulator"
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define WINDOW_FPS 60

#define TIME_STEP 3600*24

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
