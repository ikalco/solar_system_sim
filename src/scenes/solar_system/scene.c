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

	data->save_text = NULL;
	data->cursor_offset = -1;

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

	if (solar_data->save_text != NULL) {
		free(solar_data->save_text);
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

void stop_editing(Data *data) {
	SDL_StopTextInput();

	if (data->save_text != NULL) free(data->save_text);
	data->cursor_offset = -1;

	data->selected_editor = NULL;
}

void start_editing(Data *data, MenuNode *node) {
	data->selected_editor = node;

	MenuText *text = data->selected_editor->node;

	data->save_text = create_string(text->text);
	data->cursor_offset = strlen(data->save_text);

	SDL_StartTextInput();
}

void handle_mouse_body_text_editor(int clicked_id, Data *data) {
	MenuNode *clicked_node = find_menu_node_id(data->root->root, clicked_id);

	if (clicked_id < BODIES_EDITOR_LIST) {
		if (data->selected_editor == NULL) return;
		stop_editing(data);
	} else {
		if (clicked_node->type != MENU_TEXT) return;
		start_editing(data, clicked_node);
	}
}

void handle_keys_body_text_editor(Data *data, SDL_Event *event) {
	if (data->selected_editor == NULL) return;

	if (event->type == SDL_KEYDOWN) {
		switch (event->key.keysym.sym) {
		case SDLK_RETURN:
			// save changed text
			break;
		case SDLK_ESCAPE:
			// discard changed text
			break;
		case SDLK_LEFT:
			// move cursor left
			break;
		case SDLK_RIGHT:
			// move cursor right
			break;
		case SDLK_BACKSPACE:
			// delete character at cursor
			break;
		}
	}

	if (event->type == SDL_TEXTINPUT) {
		MenuText *text = data->selected_editor->node;
		data->cursor_offset += strlen(event->text.text);
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

void draw_cursor(SDL_Renderer *renderer, Data *data) {
	if (data->selected_editor == NULL) return;

	MenuText *text = data->selected_editor->node;

	int text_width, text_height;
	TTF_SizeUTF8(data->root->font, text->text, &text_width, &text_height);

	SDL_Rect offset = {
		data->selected_editor->render_offset.x +
			data->selected_editor->render_offset.w + 5,
		data->selected_editor->render_offset.y,
		5,
		data->selected_editor->render_offset.h
	};

	SDL_SetRenderDrawColor(renderer, 235, 235, 235, 255);
	SDL_RenderFillRect(renderer, &offset);
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

	draw_cursor(renderer, solar_data);
}
