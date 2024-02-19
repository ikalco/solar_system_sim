#ifndef BODIES_H
#define BODIES_H

#include "viewport.h"
#include "linked_list.h"
#include <SDL2/SDL.h>

typedef struct pb {
	VectorD position;
	VectorD velocity;
	VectorD acceleration;
	VectorD net_force;
	Color color;
	double mass;
	char* name;
} PhysicalBody;

PhysicalBody* create_body(double x, double y, double mass, Color color, char* name);
void free_body(PhysicalBody* body);
void free_bodies(List* bodies);
void draw_bodies(SDL_Renderer* renderer, Viewport* viewport, List* bodies);
void update_bodies(List* bodies, double time_step);
void print_phyiscal_body(PhysicalBody* body);

#endif
