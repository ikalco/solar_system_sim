#include "menu.h"

/*
MenuRoot (MenuList)
	MenuText
	MenuList
		MenuButton
		MenuButton
		MenuButton
	MenuButton
*/

MenuNode *init_menu_sub_list(MenuNode *root_node) {
	Color button_color = (Color){130, 130, 130, 255};
	Color text_color = {235, 235, 235, 255};

	MenuVerticalList *sub_list = create_menu_vlist((Color){50, 50, 50, 240}, (VectorD){5, 5}, 10);
	MenuNode *sub_list_node = create_menu_node(
		(VectorD){0, 0}, (VectorD){MENU_MAX_SIZE, 0}, root_node, MENU_LIST, sub_list
	);

	MenuButton *button1 =
		create_menu_button(button_color, text_color, TEXT_CENTER, create_string("Button 1"));
	MenuNode *button1_node = create_menu_node(
		(VectorD){0, 0}, (VectorD){MENU_MAX_SIZE, 60}, sub_list_node, MENU_BUTTON, button1
	);
	add_menu_vlist(sub_list, button1_node);

	MenuButton *button2 =
		create_menu_button(button_color, text_color, TEXT_CENTER, create_string("Button 2"));
	MenuNode *button2_node = create_menu_node(
		(VectorD){0, 0}, (VectorD){MENU_MAX_SIZE, 60}, sub_list_node, MENU_BUTTON, button2
	);
	add_menu_vlist(sub_list, button2_node);

	MenuButton *button3 =
		create_menu_button(button_color, text_color, TEXT_CENTER, create_string("Button 3"));
	MenuNode *button3_node = create_menu_node(
		(VectorD){0, 0}, (VectorD){MENU_MAX_SIZE, 60}, sub_list_node, MENU_BUTTON, button3
	);
	add_menu_vlist(sub_list, button3_node);

	return sub_list_node;
}

MenuRoot *init_main_menu(SDL_Window *window) {
	Color text_color = {235, 235, 235, 255};

	MenuVerticalList *root_list =
		create_menu_vlist((Color){40, 40, 40, 255}, (VectorD){20, 20}, 10);
	MenuNode *root_node =
		create_menu_node((VectorD){10, 10}, (VectorD){260, 460}, NULL, MENU_LIST, root_list);

	MenuText *title_text = create_menu_text(text_color, TEXT_CENTER, create_string("Main Menu"));
	MenuNode *title_text_node = create_menu_node(
		(VectorD){0, 0}, (VectorD){MENU_MAX_SIZE, 30}, root_node, MENU_TEXT, title_text
	);
	add_menu_vlist(root_list, title_text_node);

	MenuNode *sub_list_node = init_menu_sub_list(root_node);
	add_menu_vlist(root_list, sub_list_node);

	MenuButton *exit_button = create_menu_button(
		(Color){240, 10, 10, 255}, text_color, TEXT_CENTER, create_string("Exit")
	);
	MenuNode *exit_button_node =
		create_menu_node((VectorD){0, 0}, (VectorD){120, 50}, root_node, MENU_BUTTON, exit_button);
	add_menu_vlist(root_list, exit_button_node);

	MenuRoot *root = create_menu_root(
		window, (VectorD){100, 100}, (VectorD){300, 500}, "fonts/Roboto-Regular.ttf", root_node
	);

	render_menu_root(root);

	return root;
}
