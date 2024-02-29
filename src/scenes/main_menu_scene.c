#include "main_menu_scene.h"
#include "menu/menu.h"
#include "menu/menu_util.h"
#include "scene.h"

/*
MenuRoot (MenuList) id=0
	MenuText		id=1
	MenuList		id=2
		MenuButton	id=3
		MenuButton	id=4
		MenuButton	id=5
	MenuButton		id=6
*/

#define ROOT_LIST 0
#define MENUTEXT 1
#define SUB_LIST 2
#define CREATE_BUTTON 3
#define LOAD_BUTTON 4
#define SETTINGS_BUTTON 5
#define EXIT_BUTTON 6

typedef struct {
	SceneManager *manager;
	SDL_Window *window;
	MenuRoot *root;
	int clicked_id;
} Data;

MenuNode *init_menu_sub_list(MenuNode *root_node) {
	Color button_color = (Color){130, 130, 130, 255};
	Color text_color = {235, 235, 235, 255};

	MenuVerticalList *sub_list =
		create_menu_vlist((Color){50, 50, 50, 240}, (VectorD){5, 5}, 10);
	MenuNode *sub_list_node = create_menu_node(
		SUB_LIST,
		(VectorD){0, 0},
		(VectorD){MENU_MAX_SIZE, 0},
		root_node,
		MENU_LIST,
		sub_list
	);

	MenuButton *create_button = create_menu_button(
		button_color,
		text_color,
		TEXT_CENTER,
		create_string("Create Solar System")
	);
	MenuNode *create_button_node = create_menu_node(
		CREATE_BUTTON,
		(VectorD){0, 0},
		(VectorD){MENU_MAX_SIZE, 60},
		sub_list_node,
		MENU_BUTTON,
		create_button
	);
	add_menu_vlist(sub_list, create_button_node);

	MenuButton *load_button = create_menu_button(
		button_color,
		text_color,
		TEXT_CENTER,
		create_string("Load Solar System")
	);
	MenuNode *load_button_node = create_menu_node(
		LOAD_BUTTON,
		(VectorD){0, 0},
		(VectorD){MENU_MAX_SIZE, 60},
		sub_list_node,
		MENU_BUTTON,
		load_button
	);
	add_menu_vlist(sub_list, load_button_node);

	MenuButton *settings_button = create_menu_button(
		button_color, text_color, TEXT_CENTER, create_string("Settings")
	);
	MenuNode *settings_button_node = create_menu_node(
		SETTINGS_BUTTON,
		(VectorD){0, 0},
		(VectorD){MENU_MAX_SIZE, 60},
		sub_list_node,
		MENU_BUTTON,
		settings_button
	);
	add_menu_vlist(sub_list, settings_button_node);

	return sub_list_node;
}

MenuRoot *init_main_menu_root(SDL_Window *window) {
	Color text_color = {235, 235, 235, 255};

	MenuVerticalList *root_list =
		create_menu_vlist((Color){40, 40, 40, 255}, (VectorD){20, 20}, 10);
	MenuNode *root_node = create_menu_node(
		ROOT_LIST,
		(VectorD){10, 10},
		(VectorD){260, 460},
		NULL,
		MENU_LIST,
		root_list
	);

	MenuText *title_text =
		create_menu_text(text_color, TEXT_CENTER, create_string("Main Menu"));
	MenuNode *title_text_node = create_menu_node(
		MENUTEXT,
		(VectorD){0, 0},
		(VectorD){MENU_MAX_SIZE, 30},
		root_node,
		MENU_TEXT,
		title_text
	);
	add_menu_vlist(root_list, title_text_node);

	MenuNode *sub_list_node = init_menu_sub_list(root_node);
	add_menu_vlist(root_list, sub_list_node);

	MenuButton *exit_button = create_menu_button(
		(Color){240, 10, 10, 255},
		text_color,
		TEXT_CENTER,
		create_string("Exit")
	);
	MenuNode *exit_button_node = create_menu_node(
		EXIT_BUTTON,
		(VectorD){0, 0},
		(VectorD){120, 50},
		root_node,
		MENU_BUTTON,
		exit_button
	);
	add_menu_vlist(root_list, exit_button_node);

	MenuRoot *root = create_menu_root(
		window,
		(VectorD){100, 100},
		(VectorD){300, 500},
		"fonts/Roboto-Regular.ttf",
		root_node
	);

	render_menu_root(root);

	return root;
}

void init_main_menu(SceneManager *manager, Scene *scene, SDL_Window *window) {
	Data *data = malloc(sizeof(Data));
	data->manager = manager;
	data->window = window;
	data->root = init_main_menu_root(window);
	data->clicked_id = -1;

	scene->data = data;
}

void cleanup_main_menu(void *data) {
	Data *menu_data = data;

	if (menu_data->root != NULL) {
		free_menu_root(menu_data->root);
	}

	free(menu_data);
}

void handle_input_main_menu(void *data, SDL_Event *event) {
	Data *menu_data = data;

	if (event->type == SDL_MOUSEBUTTONDOWN) {
		menu_data->clicked_id = find_mouse_menu_root(
			menu_data->root, event->button.x, event->button.y
		);
	} else if (event->type == SDL_MOUSEBUTTONUP) {
		int new_clicked_id = find_mouse_menu_root(
			menu_data->root, event->button.x, event->button.y
		);

		// if we clicked something but then dragged mouse off and let go
		// somewhere else, then just don't do anything
		if (menu_data->clicked_id != new_clicked_id) return;

		switch (menu_data->clicked_id) {
		case EXIT_BUTTON:
			exit(0);
		case CREATE_BUTTON:
			break;
		case LOAD_BUTTON:
			select_scene_manager(menu_data->manager, menu_data->window, 1);
			break;
		case SETTINGS_BUTTON:
			break;
		default:
			break;
		}

		menu_data->clicked_id = -1;
	}
}

void draw_main_menu(void *data, SDL_Renderer *renderer) {
	Data *menu_data = data;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	draw_menu_root(menu_data->root);
}
