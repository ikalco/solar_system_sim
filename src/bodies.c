#include "bodies.h"
#include "viewport.h"

#include "math.h"

List* init_bodies_list() {	
	PhysicalBody* obj1 = malloc(sizeof(PhysicalBody));
	obj1->mass = 250e28 * KG;
	obj1->position.x = 0 * M;
	obj1->position.y = 0 * M;
	obj1->velocity.x = 0.0 * M_S;
	obj1->velocity.y = 2.34 * KM_S;
	obj1->acceleration.x = 0;
	obj1->acceleration.y = 0;
	obj1->net_force.x = 0;
	obj1->net_force.y = 0;
	obj1->color.red = 255;
	obj1->color.green = 255;
	obj1->color.blue = 0;

	PhysicalBody* obj2 = malloc(sizeof(PhysicalBody));
	obj2->mass = 25e28 * KG;
	obj2->position.x = 2 * AU;
	obj2->position.y = 0 * M;
	obj2->velocity.x = 0 * M;
	obj2->velocity.y = -23.45 * KM_S;
	obj2->acceleration.x = 0;
	obj2->acceleration.y = 0;
	obj2->net_force.x = 0;
	obj2->net_force.y = 0;
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

		rect.x = (body->position.x * viewport->conversion) + viewport->offset.x;
		rect.y = (body->position.y * viewport->conversion) + viewport->offset.y;

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
void update_body_gravity(List* bodies, PhysicalBody* body, double delta_time) {
	for (Node* other_node = bodies->first; other_node != NULL; other_node = other_node->next) {
		PhysicalBody* other = (PhysicalBody*)other_node->data;
		if (body == other) continue;

		// F is the gravitational force
		// m_1 is our mass, m_2 is the other's mass
		// R is the distance between the objects
		// gravitational equation [ F = (G*m_1*m_2) / (R^2) ]

		// if we substitute in R for distance
		// we get [ F = (G*m_1*m_2) / (sqrt((x_2-x_1)^2 + (y_2-y_1)^2))^2)]
		// ;o;  the square root cancels out!
		// so the final simplified equation is this
		// [ a = (G*m_1*m_2) / ((x_2-x_1)^2) + (y_2 - y_1)^2]

		double dist = dist_bodies_squared(body, other);
		double angle = angle_bodies(body, other);
		double g_force_mag = (G_CONSTANT * body->mass * other->mass) / dist;

		body->net_force.x += g_force_mag * cos(angle) * delta_time;
		body->net_force.y += g_force_mag * sin(angle) * delta_time;
	}
}

void update_bodies(List* bodies, double delta_time) {
	for (Node* current_node = bodies->first; current_node != NULL; current_node = current_node->next) {
		PhysicalBody* body = (PhysicalBody*)current_node->data;

		update_body_gravity(bodies, body, delta_time);

		body->acceleration.x += body->net_force.x / body->mass;
		body->acceleration.y += body->net_force.y / body->mass;

		body->velocity.x += body->acceleration.x * delta_time;
		body->velocity.y += body->acceleration.y * delta_time;

		body->position.x += body->velocity.x * delta_time;
		body->position.y += body->velocity.y * delta_time;
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
