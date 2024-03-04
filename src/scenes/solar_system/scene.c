#include "scene.h"
#include "menu.h"

#include "engine/bodies.h"
#include "engine/save.h"
#include "engine/viewport.h"
#include "menu/menu_util.h"
#include "options.h"

PhysicalBody *get_body_from_node_id(Data *data, int clicked_id) {
	Node *current = data->bodies->first;
	for (int i = 0; i < clicked_id - BODIES_BUTTONS; i++) {
		if (current == NULL) {
			printf("Trying to find unknown body.\n");
			exit(1);
		}

		current = current->next;
	}

	return (PhysicalBody *)(current->data);
}

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
		(VectorD){400, 0},
		(VectorD){WINDOW_WIDTH - 400, WINDOW_HEIGHT},
		3.5 * AU
	);
	data->name = scene->data;
	data->root = init_solar_system_menu_root(window, data);

	data->selected_body_id = BODIES_BUTTONS;
	data->selected_body = get_body_from_node_id(data, data->selected_body_id);

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

	if (solar_data->name != NULL) {
		free(solar_data->name);
	}

	if (solar_data->root != NULL) {
		free_menu_root(solar_data->root);
	}

	free(solar_data);
}

void handle_input_solar_system(void *data, SDL_Event *event) {
	Data *solar_data = data;

	int clicked_id = menu_has_clicked(solar_data->root, event);

	if (clicked_id >= BODIES_BUTTONS) {
		solar_data->selected_body =
			get_body_from_node_id(solar_data, clicked_id);
		solar_data->selected_body_id = clicked_id;
	}
}

void draw_solar_system(void *data, SDL_Renderer *renderer) {
	Data *solar_data = data;

	update_bodies(solar_data->bodies, TIME_STEP);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
	SDL_RenderFillRect(renderer, &(SDL_Rect){395, 0, 10, WINDOW_HEIGHT});

	draw_viewport_grid(renderer, solar_data->viewport);
	draw_bodies(renderer, solar_data->viewport, solar_data->bodies);

	draw_menu_root(solar_data->root);
}
