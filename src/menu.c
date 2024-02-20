#include "menu.h"

/*
MenuRoot (MenuList)
	MenuText
	MenuList
		MenuButton
		MenuSpacer
		MenuButton
		MenuButton
	MenuButton
*/

MenuRoot* init_menu() {
	MenuVerticalList* list = create_menu_vlist((Color){40, 40, 40, 255}, (VectorD){20, 20}, 10);

	MenuNode* root_node = create_menu_node((VectorD){10, 10}, (VectorD){260, 460}, MENU_LIST, list);
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
