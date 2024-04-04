#include "menu.h"
#include "options.h"

#include "engine/bodies.h"

// TODO: make some sort of scrolling or searching system in order to allow
// interaction with more that 5 bodies

void add_bodies_list_bodies(MenuNode *bodies_list_node, List *bodies) {
	MenuList *bodies_list = bodies_list_node->node;
	int body_id = BODIES_BUTTONS;

	// yes this loops backwards through the bodies, might want to fix later
	for (Node *current = bodies->first; current != NULL;
		 current = current->next) {
		// only draw 5 bodies
		if (body_id == BODIES_BUTTONS + NUM_BODIES_BUTTONS) {
			break;
		}

		PhysicalBody *body = current->data;

		MenuButton *body_button =
			create_menu_button(DEFAULT_BUTTON_COLOR,
							   (Color){235, 235, 235, 255},
							   TEXT_CENTER,
							   create_string(body->name));

		MenuNode *body_button_node =
			create_menu_node(body_id,
							 (VectorD){0, 0},
							 (VectorD){MENU_MAX_SIZE, 40},
							 bodies_list_node,
							 MENU_BUTTON,
							 body_button);

		if (body_id == BODIES_LIST) {
			body_button_node->offset.y = 5;
		}

		add_menu_list(bodies_list, body_button_node);

		body_id++;
	}
}

void add_bodies_list(MenuNode *root_node, Data *data) {
	MenuList *bodies_list = create_menu_list(
		(Color){35, 35, 35, 255}, (VectorD){10, 10}, 5, MENU_VERTICAL);

	// will only fit 5 body buttons
	MenuNode *bodies_list_node = create_menu_node(BODIES_LIST,
												  (VectorD){0, 0},
												  (VectorD){MENU_MAX_SIZE, 285},
												  root_node,
												  MENU_LIST,
												  bodies_list);

	MenuText *title_text = create_menu_text(
		(Color){235, 235, 235, 255}, TEXT_CENTER, create_string("Bodies"));
	MenuNode *title_text_node = create_menu_node(TITLE_TEXT,
												 (VectorD){0, 0},
												 (VectorD){MENU_MAX_SIZE, 35},
												 bodies_list_node,
												 MENU_TEXT,
												 title_text);
	add_menu_list(bodies_list, title_text_node);

	add_bodies_list_bodies(bodies_list_node, data->bodies);

	MenuList *root_list = root_node->node;
	add_menu_list(root_list, bodies_list_node);
}

void add_title(MenuNode *root_node, Data *data) {
	MenuList *root_list = root_node->node;
	Color text_color = {235, 235, 235, 255};

	// will probably change data->name to hold something from inisde the file
	// later rn will just use the path
	MenuText *title_text =
		create_menu_text(text_color, TEXT_CENTER, create_string(data->name));
	MenuNode *title_text_node = create_menu_node(TITLE_TEXT,
												 (VectorD){0, 0},
												 (VectorD){MENU_MAX_SIZE, 30},
												 root_node,
												 MENU_TEXT,
												 title_text);
	add_menu_list(root_list, title_text_node);

	MenuLineBreak *line_break =
		create_menu_line_break((Color){55, 55, 55, 255});
	MenuNode *line_break_node = create_menu_node(LINE_BREAK,
												 (VectorD){0, 10},
												 (VectorD){MENU_MAX_SIZE, 7},
												 root_node,
												 MENU_LINE_BREAK,
												 line_break);
	add_menu_list(root_list, line_break_node);
}

void add_bodies_editor(MenuNode *root_node, Data *data) {
	// the outline for the editor
	// the "[]" are a text editing box
	// the first 2 edit what's inside them

	// [name]      [color]
	// mass: [           ]
	// position:
	//     x: [  ] y: [  ]
	// velocity
	//     x: [  ] y: [  ]

	MenuList *root_list = root_node->node;

	MenuTextEdit *title_text = create_menu_text_edit(
		(Color){235, 235, 235, 255}, TEXT_CENTER, create_string("3.141"));
	MenuNode *title_text_node = create_menu_node(BODIES_EDITOR_LIST,
												 (VectorD){0, 0},
												 (VectorD){MENU_MAX_SIZE, 35},
												 root_node,
												 MENU_TEXT_EDIT,
												 title_text);
	add_menu_list(root_list, title_text_node);
}

MenuRoot *init_solar_system_menu_root(SDL_Window *window, Data *data) {
	MenuList *root_list = create_menu_list(
		(Color){40, 40, 40, 255}, (VectorD){20, 20}, 10, MENU_VERTICAL);
	MenuNode *root_node = create_menu_node(ROOT_LIST,
										   (VectorD){0, 0},
										   (VectorD){400, WINDOW_HEIGHT},
										   NULL,
										   MENU_LIST,
										   root_list);

	MenuRoot *root = create_menu_root(window,
									  (VectorD){0, 0},
									  (VectorD){400, WINDOW_HEIGHT},
									  "fonts/Roboto-Regular.ttf",
									  root_node);

	add_title(root_node, data);

	add_bodies_list(root_node, data);

	add_bodies_editor(root_node, data);

	render_menu_root(root);

	return root;
}
