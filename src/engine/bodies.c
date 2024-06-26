#include "bodies.h"
#include "physical.h"

#include "math.h"

PhysicalBody *
create_body(double x, double y, double mass, Color color, char *name) {
	PhysicalBody *body = malloc(sizeof(PhysicalBody));

	body->position.x = x;
	body->position.y = y;
	body->mass = mass;
	body->color = color;
	body->name = name;

	body->velocity.x = 0;
	body->velocity.y = 0;
	body->acceleration.x = 0;
	body->acceleration.y = 0;
	body->net_force.x = 0;
	body->net_force.y = 0;

	return body;
}

void free_body(PhysicalBody *body) {
	if (body == NULL) return;

	if (body->name != NULL) {
		free(body->name);
		body->name = NULL;
	}

	free(body);
}

void free_bodies(List *bodies) {
	for (Node *current_node = bodies->first; current_node != NULL;
		 current_node = current_node->next) {
		free_body(current_node->data);
		current_node->data = NULL;
	}

	free_list(bodies);
}

void draw_bodies(SDL_Renderer *renderer, Viewport *viewport, List *bodies) {
	PhysicalBody *body;
	SDL_Rect rect;
	rect.w = 25;
	rect.h = 25;

	for (Node *current_node = bodies->first; current_node != NULL;
		 current_node = current_node->next) {
		body = (PhysicalBody *)current_node->data;

		SDL_SetRenderDrawColor(renderer,
							   body->color.red,
							   body->color.green,
							   body->color.blue,
							   body->color.alpha);

		rect.x = (body->position.x * viewport->conversion) + viewport->offset.x;
		rect.y =
			(body->position.y * viewport->conversion) * -1 + viewport->offset.y;

		if (rect.x + rect.w < viewport->position.x ||
			rect.x - rect.w > viewport->position.x + viewport->size.x ||
			rect.y + rect.h < viewport->position.y ||
			rect.y - rect.h > viewport->position.y + viewport->size.y)
			continue;

		SDL_RenderFillRect(renderer, &rect);
	}
}

double dist_bodies(PhysicalBody *body1, PhysicalBody *body2) {
	double x_off = body2->position.x - body1->position.x;
	double y_off = body2->position.y - body1->position.y;

	return sqrt((x_off * x_off) + (y_off * y_off));
}

double angle_bodies(PhysicalBody *from, PhysicalBody *to) {
	double x_off = to->position.x - from->position.x;
	double y_off = to->position.y - from->position.y;

	return atan2(y_off, x_off);
}

// yes this is basically O(n^2) will optimize later
void update_body_gravity(List *bodies, PhysicalBody *body) {
	for (Node *other_node = bodies->first; other_node != NULL;
		 other_node = other_node->next) {
		PhysicalBody *other = (PhysicalBody *)other_node->data;
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

		double dist = dist_bodies(body, other);
		double angle = angle_bodies(body, other);
		double g_force_mag =
			(G_CONSTANT * body->mass * other->mass) / (dist * dist);

		body->net_force.x += g_force_mag * cos(angle);
		body->net_force.y += g_force_mag * sin(angle);
	}
}

void update_bodies(List *bodies, double time_step) {
	for (Node *current_node = bodies->first; current_node != NULL;
		 current_node = current_node->next) {
		PhysicalBody *body = (PhysicalBody *)current_node->data;

		body->net_force.x = 0;
		body->net_force.y = 0;

		update_body_gravity(bodies, body);

		body->velocity.x += (body->net_force.x / body->mass) * time_step;
		body->velocity.y += (body->net_force.y / body->mass) * time_step;

		body->position.x += body->velocity.x * time_step;
		body->position.y += body->velocity.y * time_step;
	}
}

void print_phyiscal_body(PhysicalBody *body) {
	printf("(%s) pos_x %e | pos_y %e | vel_x %e | vel_y %e | mass %e\n",
		   body->name,
		   body->position.x,
		   body->position.y,
		   body->velocity.x,
		   body->velocity.y,
		   body->mass);
}
