#ifndef BODIES_H
#define BODIES_H

#include "defines.h"

typedef struct pb {
	VectorD position;
	VectorD velocity;
	Color color;
	double mass;
} PhysicalBody;

void init_bodies_list();
void draw_bodies(List* bodies);
void print_phyiscal_body(char* name, PhysicalBody* body);

#endif
