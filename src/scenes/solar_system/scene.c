#include "scene.h"
#include "menu.h"

#include "engine/bodies.h"
#include "engine/save.h"
#include "engine/viewport.h"
#include "menu/menu_text_edit.h"
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

void set_scientific_editor(MenuRoot *root,
						   int decimal_id,
						   int exponent_id,
						   double value) {
	// might add decimal percision parameter later

	MenuTextEdit *text_edit = find_menu_node_id(root->root, decimal_id)->node;

	char *sci = text_edit->text;

	snprintf(text_edit->text, MENU_TEXT_EDIT_SIZE, "%.5e", value);

	text_edit = find_menu_node_id(root->root, exponent_id)->node;

	int offset = value < 0 ? 8 : 7;

	// probably don't need to null terminate since MenuTextEdit->text is
	// allocated to 16 chars and we're copying 4 chars
	strncpy(text_edit->text, sci + offset, 4);

	// cut off decimal at e+ since its shown by exponent
	sci[offset] = 0;
}

void set_body_editor_fields(MenuRoot *root, PhysicalBody *body) {
	set_scientific_editor(root,
						  BODIES_EDITOR_X_DECIMAL_EDIT,
						  BODIES_EDITOR_X_EXPONENT_EDIT,
						  body->position.x);

	set_scientific_editor(root,
						  BODIES_EDITOR_Y_DECIMAL_EDIT,
						  BODIES_EDITOR_Y_EXPONENT_EDIT,
						  body->position.y);

	set_scientific_editor(root,
						  BODIES_EDITOR_X_VEL_DECIMAL_EDIT,
						  BODIES_EDITOR_X_VEL_EXPONENT_EDIT,
						  body->velocity.x);

	set_scientific_editor(root,
						  BODIES_EDITOR_Y_VEL_DECIMAL_EDIT,
						  BODIES_EDITOR_Y_VEL_EXPONENT_EDIT,
						  body->velocity.y);
}

#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"

#define WRITE_BODY_SCIENTIFIC(d, e, p)                                         \
	case d:                                                                    \
		other = find_menu_node_id(root->root, e)->node;                        \
	case e:                                                                    \
		if (other == NULL) {                                                   \
			other = text_edit;                                                 \
			text_edit = find_menu_node_id(root->root, d)->node;                \
		}                                                                      \
		sscanf(text_edit->text, "%lf", &coefficient);                          \
		sscanf(other->text, "e+%lf", &exponent);                               \
		p = coefficient * pow(10, exponent);                                   \
		break;

// this uses macro just above for more concise code
void write_body_editor(MenuRoot *root,
					   PhysicalBody *body,
					   MenuNode *body_node,
					   MenuNode *editor) {
	if (editor->id < BODIES_EDITOR_START || editor->id > BODIES_EDITOR_LAST)
		return;

	if (editor->type != MENU_TEXT_EDIT) return;

	MenuTextEdit *text_edit = editor->node;
	MenuTextEdit *other = NULL;
	double coefficient, exponent;

	switch (editor->id) {
	case BODIES_EDITOR_TITLE:;
		int new_len = strlen(text_edit->text);
		body->name = realloc(body->name, sizeof(char) * new_len + 1);
		strncpy(body->name, text_edit->text, new_len);
		body->name[new_len] = 0;

		// if the title changed, change it in the body selector also
		MenuButton *body_selector_title = body_node->node;
		body_selector_title->text =
			realloc(body_selector_title->text, sizeof(char) * new_len + 1);
		strncpy(body_selector_title->text, text_edit->text, new_len);
		body_selector_title->text[new_len] = 0;
		break;
		WRITE_BODY_SCIENTIFIC(BODIES_EDITOR_MASS_DECIMAL_EDIT,
							  BODIES_EDITOR_MASS_EXPONENT_EDIT,
							  body->mass)
		WRITE_BODY_SCIENTIFIC(BODIES_EDITOR_X_DECIMAL_EDIT,
							  BODIES_EDITOR_X_EXPONENT_EDIT,
							  body->position.x)
		WRITE_BODY_SCIENTIFIC(BODIES_EDITOR_Y_DECIMAL_EDIT,
							  BODIES_EDITOR_Y_EXPONENT_EDIT,
							  body->position.y)
		WRITE_BODY_SCIENTIFIC(BODIES_EDITOR_X_VEL_DECIMAL_EDIT,
							  BODIES_EDITOR_X_VEL_EXPONENT_EDIT,
							  body->velocity.x)
		WRITE_BODY_SCIENTIFIC(BODIES_EDITOR_Y_VEL_DECIMAL_EDIT,
							  BODIES_EDITOR_Y_VEL_EXPONENT_EDIT,
							  body->velocity.y)
	default:
		break;
	}

	// write to all fields as well
	text_edit = find_menu_node_id(root->root, BODIES_EDITOR_TITLE)->node;

	strncpy(text_edit->text, body->name, MENU_TEXT_EDIT_SIZE);
	text_edit->text[MENU_TEXT_EDIT_SIZE] = 0;

	set_scientific_editor(root,
						  BODIES_EDITOR_MASS_DECIMAL_EDIT,
						  BODIES_EDITOR_MASS_EXPONENT_EDIT,
						  body->mass);

	set_body_editor_fields(root, body);
}

#pragma GCC diagnostic warning "-Wimplicit-fallthrough"

void handle_select_body(int clicked_id, Data *data) {
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

	MenuTextEdit *text_edit =
		find_menu_node_id(data->root->root, BODIES_EDITOR_TITLE)->node;

	strncpy(text_edit->text, data->selected_body->name, MENU_TEXT_EDIT_SIZE);
	text_edit->text[MENU_TEXT_EDIT_SIZE] = 0;

	set_scientific_editor(data->root,
						  BODIES_EDITOR_MASS_DECIMAL_EDIT,
						  BODIES_EDITOR_MASS_EXPONENT_EDIT,
						  data->selected_body->mass);

	set_body_editor_fields(data->root, data->selected_body);

	render_menu_root(data->root);
}

void handle_select_text_editor(int clicked_id, Data *data) {
	MenuNode *clicked_node = find_menu_node_id(data->root->root, clicked_id);

	if (clicked_id < BODIES_EDITOR_START || clicked_id > BODIES_EDITOR_LAST ||
		clicked_node->type != MENU_TEXT_EDIT) {
		if (data->selected_editor == NULL) return;
		menu_text_edit_stop_edit(data->root, data->selected_editor->node);
		return;
	}

	if (clicked_id != BODIES_EDITOR_TITLE &&
		clicked_id != BODIES_EDITOR_MASS_DECIMAL_EDIT &&
		clicked_id != BODIES_EDITOR_MASS_EXPONENT_EDIT)
		data->run_solar_system = false;

	// we clicked off so write values from body editor
	if (data->selected_editor != NULL) {
		menu_text_edit_stop_edit(data->root, data->selected_editor->node);
		write_body_editor(data->root,
						  data->selected_body,
						  data->selected_body_node,
						  data->selected_editor);
	}

	data->selected_editor = clicked_node;
	menu_text_edit_start_edit(data->root, data->selected_editor->node);
}

void handle_solar_editor(int clicked_id, Data *data) {
	if (clicked_id < SOLAR_EDITOR_START) return;

	if (clicked_id == SOLAR_EDITOR_BACK) {
		select_scene_manager(data->manager, data->window, SCENE_LOAD_MENU_ID);

		// TODO: somehow figure out own scene id
		//       this would require a kind of big refactor so,
		//       will 2 for now since it shouldn't be possible to be different
		remove_scene_manager(data->manager, 2);
		return;
	}

	if (clicked_id == SOLAR_EDITOR_PLAY_TOGGLE) {
		data->run_solar_system = data->run_solar_system ? false : true;
		return;
	}

	MenuText *playback_speed =
		find_menu_node_id(data->root->root, SOLAR_EDITOR_SPEED)->node;

	if (clicked_id == SOLAR_EDITOR_SLOW1) {
		data->playback_speed *= 0.25;
	}

	if (clicked_id == SOLAR_EDITOR_SLOW2) {
		data->playback_speed *= 0.5;
	}

	if (clicked_id == SOLAR_EDITOR_FAST1) {
		data->playback_speed *= 2;
	}

	if (clicked_id == SOLAR_EDITOR_FAST2) {
		data->playback_speed *= 4;
	}

	snprintf(playback_speed->text,
			 MENU_TEXT_EDIT_SIZE,
			 data->playback_speed < 1 ? "%.6fx" : "%.0fx",
			 data->playback_speed);
}

void handle_input_solar_system(void *data, SDL_Event *event) {
	Data *solar_data = data;

	if (solar_data->selected_editor != NULL) {
		menu_text_edit_handle_events(
			solar_data->root, solar_data->selected_editor->node, event);

		// stopped editing, so selected_editor should be null
		if (((MenuTextEdit *)solar_data->selected_editor->node)->selected ==
			false) {
			write_body_editor(solar_data->root,
							  solar_data->selected_body,
							  solar_data->selected_body_node,
							  solar_data->selected_editor);
			solar_data->selected_editor = NULL;
		}
	}

	int clicked_id = menu_has_clicked(solar_data->root, event);
	if (clicked_id != -1) {
		handle_select_text_editor(clicked_id, solar_data);
		handle_select_body(clicked_id, solar_data);

		handle_solar_editor(clicked_id, solar_data);
	}
}

// this expects scene->data to be a char* string containing a save file name
void init_solar_system(SceneManager *manager,
					   Scene *scene,
					   SDL_Window *window) {
	if (scene->data == NULL) {
		printf("Failed to load solar system scene\n");
		exit(1);
	}

	Data *data = malloc(sizeof(Data));

	data->manager = manager;
	data->window = window;
	data->filename = scene->data; // see above
	data->playback_speed = 1;
	data->run_solar_system = true;
	data->bodies = read_save_file(data->filename);

	data->selected_body = NULL;
	data->selected_body_node = NULL;
	data->selected_editor = NULL;

	data->viewport = init_viewport((VectorD){405, 0},
								   (VectorD){WINDOW_WIDTH - 405, WINDOW_HEIGHT},
								   3.5 * AU);
	data->root = init_solar_system_menu_root(window, data);

	scene->data = data;

	if (data->bodies->size == 0) return;

	handle_select_body(BODIES_BUTTONS, data);

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

	if (solar_data->root != NULL) {
		free_menu_root(solar_data->root);
	}

	if (solar_data->filename != NULL) {
		free(solar_data->filename);
	}

	free(solar_data);
}

void draw_solar_system(void *data, SDL_Renderer *renderer) {
	Data *solar_data = data;

	MenuButton *toggle =
		find_menu_node_id(solar_data->root->root, SOLAR_EDITOR_PLAY_TOGGLE)
			->node;
	if (solar_data->run_solar_system) {
		update_bodies(solar_data->bodies,
					  TIME_STEP * solar_data->playback_speed);
		set_body_editor_fields(solar_data->root, solar_data->selected_body);
		// we know toggle->text will always be 2 chars with NULL at end
		strncpy(toggle->text, "||", 2);
	} else {
		strncpy(toggle->text, ">", 2);
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	// draw_viewport_grid(renderer, solar_data->viewport);
	draw_bodies(renderer, solar_data->viewport, solar_data->bodies);

	SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
	SDL_RenderFillRect(renderer, &(SDL_Rect){395, 0, 10, WINDOW_HEIGHT});

	draw_menu_root(solar_data->root);

	// yes this is inefficient, will fix when it becomes a problem
	render_menu_root(solar_data->root);
}
