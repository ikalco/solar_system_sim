#include "scene.h"
#include "menu.h"

#include "engine/bodies.h"
#include "engine/save.h"
#include "engine/viewport.h"
#include "menu/menu_util.h"
#include "options.h"

PhysicalBody *get_body_from_node(Data *data, MenuNode *node) {
	if (node->type != MENU_BUTTON || node->id < BODIES_BUTTONS) {
		printf("Trying to find unknown body.\n");
		exit(1);
	}

	Node *current = data->bodies->first;
	for (int i = 0; i < node->id - BODIES_BUTTONS; i++) {
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

	data->selected_body = NULL;
	data->selected_body_node = NULL;

	scene->data = data;

	if (data->bodies->size == 0) return;

	data->selected_body_node =
		find_menu_node_id(data->root->root, BODIES_BUTTONS);

	data->selected_body = get_body_from_node(data, data->selected_body_node);

	MenuButton *body_button = data->selected_body_node->node;
	body_button->bg_color = SELECTED_BUTTON_COLOR;

	render_menu_root(data->root);
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

void handle_input_body_selector(int clicked_id, Data *data) {
	if (clicked_id < BODIES_BUTTONS ||
		clicked_id >= BODIES_BUTTONS + NUM_BODIES_BUTTONS)
		return;

	if (data->selected_body_node != NULL) {
		MenuButton *body_button = data->selected_body_node->node;
		body_button->bg_color = DEFAULT_BUTTON_COLOR;
	}

	data->selected_body_node = find_menu_node_id(data->root->root, clicked_id);

	data->selected_body = get_body_from_node(data, data->selected_body_node);

	MenuButton *body_button = data->selected_body_node->node;
	body_button->bg_color = SELECTED_BUTTON_COLOR;

	render_menu_root(data->root);
}

void handle_input_solar_system(void *data, SDL_Event *event) {
	Data *solar_data = data;

	int clicked_id = menu_has_clicked(solar_data->root, event);

	handle_input_body_selector(clicked_id, data);
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
