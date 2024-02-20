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

MenuNode* init_menu_sub_list() {
	Color text_color = {235, 235, 235, 255};

	MenuVerticalList* sub_list = create_menu_vlist((Color){50, 50, 50, 240}, (VectorD){5, 5}, 10);
	MenuNode* sub_list_node = create_menu_node((VectorD){0, 20}, (VectorD){MENU_MAX_SIZE, MENU_MAX_SIZE}, MENU_LIST, sub_list);

	return sub_list_node;
}

MenuRoot* init_menu() {
	Color text_color = {235, 235, 235, 255};

	MenuVerticalList* root_list = create_menu_vlist((Color){40, 40, 40, 255}, (VectorD){20, 20}, 10);
	MenuNode* root_node = create_menu_node((VectorD){10, 10}, (VectorD){260, 460}, MENU_LIST, root_list);

	MenuText* text1 = create_menu_text(text_color, TEXT_CENTER, "Main Menu");
	MenuNode* text1_node = create_menu_node((VectorD){0, 0}, (VectorD){MENU_MAX_SIZE, 30}, MENU_TEXT, text1);
	add_menu_vlist(root_list, text1_node);

	MenuNode* sub_list_node = init_menu_sub_list();
	add_menu_vlist(root_list, sub_list_node);

	MenuButton* button1 = create_menu_button((Color){240, 10, 10, 255}, text_color, "Exit");
	MenuNode* button1_node = create_menu_node((VectorD){0, 0}, (VectorD){120, 50}, MENU_BUTTON, button1);

	MenuRoot* root = create_menu_root((VectorD){100, 100}, (VectorD){300, 500}, "arial.ttf", root_node);

	return root;
}

MenuRoot* create_menu_root(VectorD position, VectorD size, char* fontname, MenuNode* root_node) {
	MenuRoot* root = malloc(sizeof(MenuRoot));

	root->position = position;
	root->size = size;
	root->root = root_node;

	root->font = TTF_OpenFont(fontname, 12);

	return root;
}

MenuNode* create_menu_node(VectorD offset, VectorD size, MenuType type, void* node) {
	MenuNode* ret = malloc(sizeof(MenuNode));

	ret->offset = offset;
	ret->size = size;
	ret->type = type;
	ret->node = node;

	ret->next = NULL;

	return ret;
}

MenuVerticalList* create_menu_vlist(Color bg_color, VectorD padding, double spacing) {
	MenuVerticalList* list = malloc(sizeof(MenuVerticalList));

	list->bg_color = bg_color;
	list->padding = padding;
	list->spacing = spacing;

	list->child = NULL;
	list->size = 0;

	return list;
}

void add_menu_vlist(MenuVerticalList* list, MenuNode* node) {
	node->next = list->child;
	list->child = node;
	list->size++;
}

MenuText* create_menu_text(Color text_color, MenuTextAlign align, char* text) {
	MenuText* menu_text = malloc(sizeof(MenuText));

	menu_text->text_color = text_color;
	menu_text->text = text;
	menu_text->align = align;

	return menu_text;
}
