#include "linked_list.h"
#include <stdint.h>

#define WINDOW_TITLE "An SDL2 Window"
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define WINDOW_FPS 60

// constants
#define G_CONSTANT 6.67430e-11

// units
#define M 1						// meters
#define KM 1000					// kilometers	
#define AU 149597870700			// astronomical unit (in meters)
#define G 0.001					// grams
#define KG 1					// kilograms
#define S 1						// seconds

// derived units
#define M_S 1					// meters per second
#define KM_S 1000				// kilometers per second

typedef struct vd {
	double x;
	double y;
} VectorD;

typedef struct color {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} Color;

typedef struct pb {
	VectorD position;
	VectorD velocity;
	Color color;
	double mass;
} PhysicalBody;

List* init_bodies_list();
void draw_bodies(List* bodies);
void print_phyiscal_body(char* name, PhysicalBody* body);

void initSDL();
void cleanup();
void handle_input();
