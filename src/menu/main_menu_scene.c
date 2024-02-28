#include "main_menu_scene.h"
#include "menu.h"
#include "menu_util.h"

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
#define BUTTON_1 3
#define BUTTON_2 4
#define BUTTON_3 5
#define EXIT_BUTTON 6

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

	MenuButton *button1 = create_menu_button(
		button_color, text_color, TEXT_CENTER, create_string("Button 1")
	);
	MenuNode *button1_node = create_menu_node(
		BUTTON_1,
		(VectorD){0, 0},
		(VectorD){MENU_MAX_SIZE, 60},
		sub_list_node,
		MENU_BUTTON,
		button1
	);
	add_menu_vlist(sub_list, button1_node);

	MenuButton *button2 = create_menu_button(
		button_color, text_color, TEXT_CENTER, create_string("Button 2")
	);
	MenuNode *button2_node = create_menu_node(
		BUTTON_2,
		(VectorD){0, 0},
		(VectorD){MENU_MAX_SIZE, 60},
		sub_list_node,
		MENU_BUTTON,
		button2
	);
	add_menu_vlist(sub_list, button2_node);

	MenuButton *button3 = create_menu_button(
		button_color, text_color, TEXT_CENTER, create_string("Button 3")
	);
	MenuNode *button3_node = create_menu_node(
		BUTTON_3,
		(VectorD){0, 0},
		(VectorD){MENU_MAX_SIZE, 60},
		sub_list_node,
		MENU_BUTTON,
		button3
	);
	add_menu_vlist(sub_list, button3_node);

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

void init_main_menu(Scene *scene, SDL_Window *window) {
	// init sdl2_ttf
	if (TTF_Init() < 0) {
		printf("SDL_ttf couldn't be initialized.\n");
		exit(1);
	}

	MenuRoot *root = init_main_menu_root(window);
	scene->data = root;
}

void cleanup_main_menu(void *data) {
	MenuRoot *root = data;

	if (root != NULL) {
		free_menu_root(root);
	}

	TTF_Quit();
}

void handle_input_main_menu(void *data, SDL_Event *event) {
	MenuRoot *root = data;

	// we'll only handle moust button up events
	if (event->type != SDL_MOUSEBUTTONUP) return;

	int clicked_id =
		find_mouse_menu_root(root, event->button.x, event->button.y);

	switch (clicked_id) {
	case EXIT_BUTTON:
		exit(0);
	case BUTTON_1:
		break;
	case BUTTON_2:
		break;
	case BUTTON_3:
		break;
	default:
		break;
	}
}

void draw_main_menu(void *data, SDL_Renderer *renderer) {
	MenuRoot *root = data;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	draw_menu_root(root);
}
