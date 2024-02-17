#ifndef BODIES_H
#define BODIES_H

#include "defines.h"
#include "viewport.h"

typedef struct pb {
	VectorD position;
	VectorD velocity;
	VectorD accel;
	Color color;
	double mass;
} PhysicalBody;

List* init_bodies_list();
void draw_bodies(SDL_Renderer* renderer, Viewport* viewport, List* bodies);
void update_bodies(List* bodies, double delta_time);
void print_phyiscal_body(char* name, PhysicalBody* body);

#endif
