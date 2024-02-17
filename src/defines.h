#include <SDL2/SDL.h>

#define WINDOW_TITLE "An SDL2 Window"
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define WINDOW_FPS 60

#define G_CONSTANT 6.67430e-11

typedef struct vd {
	double x;
	double y;
} VectorD;

typedef struct pb {
	VectorD position;
	VectorD velocity;
	double mass;
} PhysicalBody;

void initSDL();
void cleanup();
void handle_input();
