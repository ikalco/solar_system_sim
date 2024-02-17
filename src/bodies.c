#include "bodies.h"
#include "viewport.h"

extern SDL_Renderer *renderer;
extern Viewport* viewport;

List* bodies;

void init_bodies_list() {	
	PhysicalBody* obj1 = malloc(sizeof(PhysicalBody));
	obj1->mass = 250 * KG;
	obj1->position.x = 0 * AU;
	obj1->position.y = 0 * AU;
	obj1->velocity.x = 0 * AU;
	obj1->velocity.y = 0 * AU;
	obj1->color.red = 255;
	obj1->color.green = 255;
	obj1->color.blue = 0;

	PhysicalBody* obj2 = malloc(sizeof(PhysicalBody));
	obj2->mass = 25 * KG;
	obj2->position.x = 2 * AU;
	obj2->position.y = 0 * AU;
	obj2->velocity.x = 23.45 * KM_S;
	obj2->velocity.y = 0 * AU;
	obj2->color.red = 255;
	obj2->color.green = 0;
	obj2->color.blue = 255;

	bodies = create_list();
	add_list(bodies, (void*)obj1);
	add_list(bodies, (void*)obj2);
}

void draw_bodies(List* bodies) {
	PhysicalBody* body;
	SDL_Rect rect;
	rect.w = 25;
	rect.h = 25;

	for (Node* current_node = bodies->first; current_node != NULL; current_node = current_node->next) {
		body = (PhysicalBody*)current_node->data;

		SDL_SetRenderDrawColor(renderer, body->color.red, body->color.green, body->color.blue, 255);

		rect.x = (body->position.x * viewport->cell_size) + viewport->offset.x;
		rect.y = (body->position.y * viewport->cell_size) + viewport->offset.y;

		SDL_RenderFillRect(renderer, &rect);
	}
}

void print_phyiscal_body(char* name, PhysicalBody* body) {
	printf("(%s) pos_x %f | pos_y %f | vel_x %f | vel_y %f | mass %f\n", 
		name,
		body->position.x,
		body->position.y,
		body->velocity.x,
		body->velocity.y,
		body->mass
	);
}
