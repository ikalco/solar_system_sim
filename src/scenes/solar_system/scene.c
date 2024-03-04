#include "scene.h"

#include "engine/bodies.h"
#include "engine/save.h"
#include "engine/viewport.h"
#include "options.h"

typedef struct {
	SceneManager *manager;
	SDL_Window *window;
	List *bodies;
	Viewport *viewport;
} Data;

// this expects scene->data to be a char* string containing a save file name
void init_solar_system(
	SceneManager *manager,
	Scene *scene,
	SDL_Window *window
) {
	if (scene->data == NULL) {
		printf("Failed to load solar system scene\n");
		exit(1);
	}

	Data *data = malloc(sizeof(Data));

	data->manager = manager;
	data->window = window;
	data->bodies = read_save_file(scene->data);
	data->viewport = init_viewport(
		(VectorD){0, 0}, (VectorD){WINDOW_WIDTH, WINDOW_HEIGHT}, 3.5 * AU
	);

	scene->data = data;
}

void cleanup_solar_system(void *data) {
	Data *solar_data = data;

	if (solar_data->bodies != NULL) {
		free_bodies(solar_data->bodies);
	}

	if (solar_data->viewport != NULL) {
		free(solar_data->viewport);
	}

	free(solar_data);
}

void handle_input_solar_system(void *data, SDL_Event *event) {
	// TODO
}

void draw_solar_system(void *data, SDL_Renderer *renderer) {
	Data *solar_data = data;

	update_bodies(solar_data->bodies, TIME_STEP);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	draw_viewport_grid(renderer, solar_data->viewport);
	draw_bodies(renderer, solar_data->viewport, solar_data->bodies);
}
