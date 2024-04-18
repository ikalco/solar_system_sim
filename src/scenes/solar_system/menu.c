#include "menu.h"
#include "options.h"

#include "engine/bodies.h"

Color text_color = {235, 235, 235, 255};

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

		MenuButton *body_button = create_menu_button(DEFAULT_BUTTON_COLOR,
													 text_color,
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

	MenuText *title_text =
		create_menu_text(text_color, TEXT_CENTER, create_string("Bodies"));
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

	// will probably change data->name to hold something from inisde the file
	// later rn will just use the path
	MenuText *title_text = create_menu_text(
		text_color, TEXT_CENTER, create_string(data->filename));
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

// always make sure that id + 3 doesn't overlap other menu nodes
MenuNode *
make_scientific_editor(MenuNode *root_node, const char *editor_text, int id) {
	MenuList *list = create_menu_list(
		(Color){40, 40, 40, 255}, (VectorD){0, 0}, 20, MENU_HORIZONTAL);
	MenuNode *list_node = create_menu_node(id,
										   (VectorD){0, 0},
										   (VectorD){MENU_MAX_SIZE, 35},
										   root_node,
										   MENU_LIST,
										   list);

	MenuText *text =
		create_menu_text(text_color, TEXT_LEFT, create_string(editor_text));
	MenuNode *text_node = create_menu_node(
		id + 1, (VectorD){0, 0}, (VectorD){80, 35}, list_node, MENU_TEXT, text);
	add_menu_list(list, text_node);

	MenuTextEdit *decimal_edit =
		create_menu_text_edit(text_color, TEXT_LEFT, "");
	MenuNode *decimal_edit_node = create_menu_node(id + 2,
												   (VectorD){0, 0},
												   (VectorD){120, 35},
												   list_node,
												   MENU_TEXT_EDIT,
												   decimal_edit);
	add_menu_list(list, decimal_edit_node);

	MenuTextEdit *exponent_edit =
		create_menu_text_edit(text_color, TEXT_LEFT, "");
	MenuNode *exponent_edit_node =
		create_menu_node(id + 3,
						 (VectorD){0, 0},
						 (VectorD){MENU_MAX_SIZE, 35},
						 list_node,
						 MENU_TEXT_EDIT,
						 exponent_edit);
	add_menu_list(list, exponent_edit_node);

	return list_node;
}

// TODO: maybe make a nice color editor...
void add_bodies_editor(MenuNode *root_node) {
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

	// will add color editing later
	MenuTextEdit *title_text =
		create_menu_text_edit(text_color, TEXT_CENTER, "");
	MenuNode *title_text_node = create_menu_node(BODIES_EDITOR_TITLE,
												 (VectorD){0, 0},
												 (VectorD){MENU_MAX_SIZE, 35},
												 root_node,
												 MENU_TEXT_EDIT,
												 title_text);
	add_menu_list(root_list, title_text_node);

	MenuNode *mass_node =
		make_scientific_editor(root_node, "Mass:", BODIES_EDITOR_MASS);
	add_menu_list(root_list, mass_node);

	MenuNode *x_node =
		make_scientific_editor(root_node, "X pos:", BODIES_EDITOR_X);
	add_menu_list(root_list, x_node);

	MenuNode *y_node =
		make_scientific_editor(root_node, "Y pos:", BODIES_EDITOR_Y);
	add_menu_list(root_list, y_node);

	MenuNode *x_vel_node =
		make_scientific_editor(root_node, "X vel:", BODIES_EDITOR_X_VEL);
	add_menu_list(root_list, x_vel_node);

	MenuNode *y_vel_node =
		make_scientific_editor(root_node, "Y vel:", BODIES_EDITOR_Y_VEL);
	add_menu_list(root_list, y_vel_node);
}

// TODO: make solar system properties editor
// [<-] 1x [<<] [<] [>] [>>] [⏯]
// back arrow
// playback speed display
// change playback speed, 0.25x, 0.5x, 2x, 4x
// pause/play, ⏵, ⏸, ⏯

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

	add_bodies_editor(root_node);

	render_menu_root(root);

	return root;
}
