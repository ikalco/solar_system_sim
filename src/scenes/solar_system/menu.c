#include "menu.h"
#include "options.h"

#include "engine/bodies.h"

// TODO: make some sort of scrolling or searching system in order to allow
// interaction with more that 5 bodies

void add_bodies_list(MenuNode *bodies_list_node, List *bodies) {
	MenuVerticalList *bodies_list = bodies_list_node->node;
	int body_id = BODIES_BUTTONS;

	// yes this loops backwards through the bodies, might want to fix later
	for (Node *current = bodies->first; current != NULL;
		 current = current->next) {
		// only draw 5 bodies
		if (body_id == BODIES_BUTTONS + 5) {
			break;
		}

		PhysicalBody *body = current->data;

		MenuButton *body_button = create_menu_button(
			(Color){40, 40, 40, 255},
			(Color){235, 235, 235, 255},
			TEXT_CENTER,
			create_string(body->name)
		);

		MenuNode *body_button_node = create_menu_node(
			body_id,
			(VectorD){0, 0},
			(VectorD){MENU_MAX_SIZE, 40},
			bodies_list_node,
			MENU_BUTTON,
			body_button
		);

		if (body_id == BODIES_LIST) {
			body_button_node->offset.y = 5;
		}

		add_menu_vlist(bodies_list, body_button_node);

		body_id++;
	}
}

MenuNode *init_bodies_list(MenuNode *root_node, Data *data) {
	MenuVerticalList *bodies_list =
		create_menu_vlist((Color){35, 35, 35, 255}, (VectorD){10, 10}, 5);

	// will only fit 5 body buttons
	MenuNode *bodies_list_node = create_menu_node(
		BODIES_LIST,
		(VectorD){0, 0},
		(VectorD){MENU_MAX_SIZE, 285},
		root_node,
		MENU_LIST,
		bodies_list
	);

	MenuText *title_text = create_menu_text(
		(Color){235, 235, 235, 255}, TEXT_CENTER, create_string("Bodies")
	);
	MenuNode *title_text_node = create_menu_node(
		TITLE_TEXT,
		(VectorD){0, 0},
		(VectorD){MENU_MAX_SIZE, 35},
		bodies_list_node,
		MENU_TEXT,
		title_text
	);
	add_menu_vlist(bodies_list, title_text_node);

	add_bodies_list(bodies_list_node, data->bodies);

	return bodies_list_node;
}

MenuRoot *init_solar_system_menu_root(SDL_Window *window, Data *data) {
	Color text_color = {235, 235, 235, 255};

	MenuVerticalList *root_list =
		create_menu_vlist((Color){40, 40, 40, 255}, (VectorD){20, 20}, 10);
	MenuNode *root_node = create_menu_node(
		ROOT_LIST,
		(VectorD){0, 0},
		(VectorD){400, WINDOW_HEIGHT},
		NULL,
		MENU_LIST,
		root_list
	);

	// will probably change data->name to hold something from inisde the file
	// later rn will just use the path
	MenuText *title_text =
		create_menu_text(text_color, TEXT_CENTER, create_string(data->name));
	MenuNode *title_text_node = create_menu_node(
		TITLE_TEXT,
		(VectorD){0, 0},
		(VectorD){MENU_MAX_SIZE, 30},
		root_node,
		MENU_TEXT,
		title_text
	);
	add_menu_vlist(root_list, title_text_node);

	MenuLineBreak *line_break =
		create_menu_line_break((Color){55, 55, 55, 255});
	MenuNode *line_break_node = create_menu_node(
		LINE_BREAK,
		(VectorD){0, 10},
		(VectorD){MENU_MAX_SIZE, 7},
		root_node,
		MENU_LINE_BREAK,
		line_break
	);
	add_menu_vlist(root_list, line_break_node);

	MenuNode *bodies_list_node = init_bodies_list(root_node, data);
	add_menu_vlist(root_list, bodies_list_node);

	MenuRoot *root = create_menu_root(
		window,
		(VectorD){0, 0},
		(VectorD){400, WINDOW_HEIGHT},
		"fonts/Roboto-Regular.ttf",
		root_node
	);

	render_menu_root(root);

	return root;
}
