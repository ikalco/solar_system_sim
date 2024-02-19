#include "menu.h"

MenuNode* create_menu_root(VectorD position, double width, double height, Color bg_color) {
	MenuNode* root = malloc(sizeof(MenuNode));

	root->position = position;
	root->width = width;
	root->height = height;
	root->bg_color = bg_color;
	root->type = MENU_NONE;
	root->node = NULL;

	return root;
}

void draw_menu(SDL_Renderer* renderer, Viewport* viewport, MenuNode* menu) {
	switch (menu->type) {
		case MENU_NONE:
			break;
		case MENU_LIST:
			draw_menu_list(renderer, viewport, menu);
			break;
		case MENU_SELECT:
			draw_menu_select(renderer, viewport, menu);
			break;
		case MENU_BUTTON:
			draw_menu_button(renderer, viewport, menu);
			break;
		case MENU_SPACER:
			draw_menu_spacer(renderer, viewport, menu);
			break;
	}
}

void free_menu_node(MenuNode* node) {
	switch (node->type) {
		case MENU_NONE:
			break;
		case MENU_LIST:
			free_menu_list(node->node);
			break;
		case MENU_SELECT:
			free_menu_select(node->node);
			break;
		case MENU_BUTTON:
			free_menu_button(node->node);
			break;
		case MENU_SPACER:
			free_menu_spacer(node->node);
			break;
	}
}

MenuList* create_menu_list(double vertical_spacing, double outer_spacing) {
	MenuList* list = malloc(sizeof(MenuList));

	list->vertical_spacing = vertical_spacing;
	list->outer_spacing = outer_spacing;

	list->size = 0;
	list->first = NULL;

	return list;
}

void add_menu_list(MenuList* list, MenuNode* node) {
	MenuListNode* new_node = malloc(sizeof(MenuListNode));
	new_node->node = node;
	new_node->next = list->first;
	list->first = new_node;
	list->size++;
}

MenuNode* remove_menu_list(MenuList* list) {
	if (list->size == 0) {
		return NULL;
	}

	MenuListNode* removed = list->first;
	MenuNode* data = removed->node;
	
	list->first = removed->next;
	list->size--;

	free(removed);

	return data;
}

void draw_menu_list(SDL_Renderer* renderer, Viewport* viewport, MenuNode* list) {
	// TODO
}

void free_menu_list(MenuList* list) {
	MenuListNode* current = list->first;
	while(current != NULL) {
		MenuListNode* next = current->next;
		
		if (current->node != NULL) {
			free_menu_node(current->node);
		}
		
		free(current);

		current = next;
	}

	free(list);
}

MenuSelect* create_menu_select(Color text_color, char* text) {
	MenuSelect* select = malloc(sizeof(MenuSelect));
	select->text_color = text_color;
	select->text = text;
	select->selected = false;
	return select;
}

void draw_menu_select(SDL_Renderer* renderer, Viewport* viewport, MenuNode* select) {
	// TODO
}

void free_menu_select(MenuSelect* select) {
	free(select->text);
	free(select);
}

MenuButton* create_menu_button(Color text_color, char* text) {
	MenuButton* button = malloc(sizeof(MenuButton));
	button->text_color = text_color;
	button->text = text;
	return button;
}

void draw_menu_button(SDL_Renderer* renderer, Viewport* viewport, MenuNode* button) {
	// TODO
}

void free_menu_button(MenuButton* button) {
	free(button->text);
	free(button);
}

MenuSpacer* create_menu_spacer(double spacing) {
	MenuSpacer* spacer = malloc(sizeof(MenuSpacer));
	spacer->spacing = spacing;
	return spacer;
}

void draw_menu_spacer(SDL_Renderer* renderer, Viewport* viewport, MenuNode* spacer) {
	// TODO
}

void free_menu_spacer(MenuSpacer* spacer) {
	free(spacer);
}
