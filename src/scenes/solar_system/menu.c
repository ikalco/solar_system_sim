#include "menu.h"
#include "options.h"

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
		(VectorD){MENU_MAX_SIZE, 10},
		root_node,
		MENU_LINE_BREAK,
		line_break
	);
	add_menu_vlist(root_list, line_break_node);

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
