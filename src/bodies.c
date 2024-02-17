#include "bodies.h"
#include "viewport.h"

#include "math.h"

List* init_bodies_list() {	
	PhysicalBody* obj1 = malloc(sizeof(PhysicalBody));
	obj1->mass = 250 * KG;
	obj1->position.x = 0 * AU;
	obj1->position.y = 0 * AU;
	obj1->velocity.x = 0 * AU;
	obj1->velocity.y = 0 * AU;
	obj1->accel.x = 0 * AU;
	obj1->accel.y = 0 * AU;
	obj1->color.red = 255;
	obj1->color.green = 255;
	obj1->color.blue = 0;

	PhysicalBody* obj2 = malloc(sizeof(PhysicalBody));
	obj2->mass = 25 * KG;
	obj2->position.x = 2 * AU;
	obj2->position.y = 0 * AU;
	obj2->velocity.x = 23.45 * KM_S;
	obj2->velocity.y = 0 * AU;
	obj1->accel.x = 0 * AU;
	obj1->accel.y = 0 * AU;
	obj2->color.red = 255;
	obj2->color.green = 0;
	obj2->color.blue = 255;

	List* bodies = create_list();
	add_list(bodies, (void*)obj1);
	add_list(bodies, (void*)obj2);

	return bodies;
}

void draw_bodies(SDL_Renderer* renderer, Viewport* viewport, List* bodies) {
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

double dist_bodies_squared(PhysicalBody* body1, PhysicalBody* body2) {
	double x_off = body2->position.x - body1->position.x;
	double y_off = body2->position.y - body1->position.y;

	return (x_off * x_off) + (y_off * y_off);
}

double angle_bodies(PhysicalBody* from, PhysicalBody* to) {
	double x_off = to->position.x - from->position.x;
	double y_off = to->position.y - from->position.y;

	return atan2(y_off, x_off);
}

// yes this is basically O(n^2) will optimize later
void update_body_accel(List* bodies, PhysicalBody* body) {
	for (Node* other_node = bodies->first; other_node != NULL; other_node = other_node->next) {
		PhysicalBody* other = (PhysicalBody*)other_node->data;
		if (body == other) continue;

		// F is the gravitational force
		// m_1 is our mass, m_2 is the other's mass
		// R is the distance between the objects
		// gravitational equation [ F = (G*m_1*m_2) / (R^2) ]

		// since we just need the acceleration of our object,
		// then we can rearrange for it by substituting [ F = m_1*a ]
		// so we get [ a = (G*m_2) / (R^2) ]

		// now substituting in R for distance
		// we get [ a = (G*m_2) / (sqrt((x_2-x_1)^2 + (y_2-y_1)^2))^2)]
		// ;o;  the square root cancels out!
		// so the final simplified equation is this
		// [ a = (G*m_2) / ((x_2-x_1)^2) + (y_2 - y_1)^2]

		double dist = dist_bodies_squared(body, other);
		double angle = angle_bodies(body, other);
		double g_accel_mag = (G_CONSTANT * other->mass) / dist;

		body->accel.x += g_accel_mag * cos(angle);
		body->accel.y += g_accel_mag * sin(angle);
	}
}

void update_bodies(List* bodies) {
	for (Node* current_node = bodies->first; current_node != NULL; current_node = current_node->next) {
		PhysicalBody* body = (PhysicalBody*)current_node->data;

		update_body_accel(bodies, body);

		body->velocity.x += body->accel.x;
		body->velocity.y += body->accel.y;

		body->position.x += body->velocity.x;
		body->position.y += body->velocity.y;
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
