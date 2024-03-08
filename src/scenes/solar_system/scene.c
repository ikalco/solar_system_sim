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

	data->selected_editor = NULL;

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

void handle_mouse_body_selector(int clicked_id, Data *data) {
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

void handle_mouse_body_text_editor(int clicked_id, Data *data) {
	MenuNode *clicked_node = find_menu_node_id(data->root->root, clicked_id);

	if (clicked_id < BODIES_EDITOR_LIST) {
		if (data->selected_editor != NULL) {
			SDL_StopTextInput();
			data->selected_editor = NULL;
		}
	} else {
		if (clicked_node->type == MENU_TEXT) {
			data->selected_editor = clicked_node;
			SDL_StartTextInput();
		}
	}
}

void handle_keys_body_text_editor(Data *data, SDL_Event *event) {
	if (data->selected_editor == NULL) return;

	if (event->type == SDL_KEYDOWN) {
		switch (event->key.keysym.sym) {
		case SDLK_RETURN:
			break;
		case SDLK_ESCAPE:
			break;
		case SDLK_LEFT:
			break;
		case SDLK_RIGHT:
			break;
		case SDLK_BACKSPACE:
			break;
		}
	}

	if (event->type == SDL_TEXTINPUT) {
		MenuText *text = data->selected_editor->node;
		strcat(text->text, event->text.text);
		render_menu_root(data->root);
	}
}

void handle_input_solar_system(void *data, SDL_Event *event) {
	Data *solar_data = data;

	int clicked_id = menu_has_clicked(solar_data->root, event);
	if (clicked_id != -1) {
		handle_mouse_body_selector(clicked_id, solar_data);
		handle_mouse_body_text_editor(clicked_id, solar_data);
	}

	handle_keys_body_text_editor(solar_data, event);
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
