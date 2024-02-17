#include <stdio.h>
#include "defines.h"

SDL_Window *window;
SDL_Renderer *renderer;

int main() {
	initSDL();
	atexit(cleanup);

	List* bodies = init_bodies_list();

	printf("size: %d\n", bodies->size);

	PhysicalBody* obj = (PhysicalBody*)(bodies->first->data);
	print_phyiscal_body("obj1", obj);
	
	obj = (PhysicalBody*)(bodies->first->next->data);
	print_phyiscal_body("obj2", obj);

	while (1) {
		handle_input();

		// clear screen with black
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		draw_viewport_grid();
		draw_bodies(bodies);

		// draw to screen and wait amount of time for desired fps
		SDL_RenderPresent(renderer);
		SDL_Delay(1000/WINDOW_FPS);
	}

	return 0;
}

void draw_bodies(List* bodies) {
	PhysicalBody* body;
	SDL_Rect rect;

	for (Node* current_node = bodies->first; current_node != NULL; current_node = current_node->next) {
		body = (PhysicalBody*)current_node->data;

		SDL_SetRenderDrawColor(renderer, body->color.red, body->color.green, body->color.blue, 255);

		rect.x = 0;
		rect.y = 0;
		rect.h = 0;
		rect.w = 0;
	}
}

void draw_viewport_grid() {
	int min_size;

	if (WINDOW_WIDTH < WINDOW_HEIGHT) {
		min_size = WINDOW_WIDTH;
	} else {
		min_size = WINDOW_HEIGHT;
	}

	double cell_size = (double) min_size / VIEWPORT_SIZE;

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	// vertical lines
	for(int i = 1; i < WINDOW_WIDTH / cell_size; i++) {
		SDL_RenderDrawLine(renderer, i * cell_size, 0, i * cell_size, WINDOW_HEIGHT);
	}

	// horizontal lines
	for(int i = 1; i < WINDOW_HEIGHT / cell_size; i++) {
		SDL_RenderDrawLine(renderer, 0, i * cell_size, WINDOW_WIDTH, i * cell_size);
	}
}

List* init_bodies_list() {	
	PhysicalBody* obj1 = malloc(sizeof(PhysicalBody));
	obj1->mass = 250 * KG;
	obj1->position.x = 0 * AU;
	obj1->position.y = 0 * AU;
	obj1->velocity.x = 0 * AU;
	obj1->velocity.y = 0 * AU;

	PhysicalBody* obj2 = malloc(sizeof(PhysicalBody));
	obj2->mass = 25 * KG;
	obj2->position.x = 2 * AU;
	obj2->position.y = 0 * AU;
	obj2->velocity.x = 23.45 * KM_S;
	obj2->velocity.y = 0 * AU;

	List* bodies_list = create_list();
	add_list(bodies_list, (void*)obj1);
	add_list(bodies_list, (void*)obj2);

	return bodies_list;
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

void handle_input() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				exit(0);
				break;
			default:
				break;
		}
	}
}

void initSDL() {
	// init sdl2
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL couldn't be initialized.\n");
		exit(1);
	}

	// create window
	window = SDL_CreateWindow(
		WINDOW_TITLE,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_SHOWN
	);

	// if window is undefined print error and exit
	if (window == NULL) {
		printf("Couldn't create window: %s\n", SDL_GetError());
		exit(1);
	}

	// create renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	// if renderer is undefined print error and exit
	if (renderer == NULL) {
		printf("Couldn't create renderer: %s\n", SDL_GetError());
		exit(1);
	}
}

void cleanup() {
	SDL_DestroyRenderer(renderer);

	SDL_DestroyWindow(window);

	// clean up sdl
	SDL_Quit();
}
