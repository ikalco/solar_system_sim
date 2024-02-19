#include "menu.h"

MenuNode* create_menu_root() {
	MenuNode* root = malloc(sizeof(MenuNode));

	root->type = MENU_NONE;
	root->node = NULL;

	return root;
}

void draw_menu_node(SDL_Renderer* renderer, MenuNode* menu) {
	switch (menu->type) {
		case MENU_NONE:
			break;
		case MENU_LIST:
			draw_menu_list(renderer, menu->node);
			break;
		case MENU_SELECT:
			draw_menu_select(renderer, menu->node);
			break;
		case MENU_BUTTON:
			draw_menu_button(renderer, menu->node);
			break;
		case MENU_SPACER:
			draw_menu_spacer(renderer, menu->node);
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

MenuList* create_menu_list(VectorD position, double width, double height, Color bg_color, double vertical_spacing, double outer_spacing) {
	MenuList* list = malloc(sizeof(MenuList));

	list->position = position;
	list->width = width;
	list->height = height;
	list->bg_color = bg_color;
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

void draw_menu_list(SDL_Renderer* renderer, MenuList* list) {
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

MenuSelect* create_menu_select(VectorD position, double width, double height, Color bg_color, Color text_color, char* text) {
	MenuSelect* select = malloc(sizeof(MenuSelect));
	
	select->position = position;
	select->width = width;
	select->height = height;
	select->bg_color = bg_color;
	select->text_color = text_color;
	select->text = text;
	select->selected = false;

	return select;
}

void draw_menu_select(SDL_Renderer* renderer, MenuSelect* select) {
	// TODO
}

void free_menu_select(MenuSelect* select) {
	free(select->text);
	free(select);
}

MenuButton* create_menu_button(VectorD position, double width, double height, Color bg_color, Color text_color, char* text) {
	MenuButton* button = malloc(sizeof(MenuButton));

	button->position = position;
	button->width = width;
	button->height = height;
	button ->bg_color = bg_color;
	button->text_color = text_color;
	button->text = text;

	return button;
}

void draw_menu_button(SDL_Renderer* renderer, MenuButton* button) {
	// TODO
}

void free_menu_button(MenuButton* button) {
	free(button->text);
	free(button);
}

MenuSpacer* create_menu_spacer(VectorD position, double width, double height, Color bg_color, double spacing) {
	MenuSpacer* spacer = malloc(sizeof(MenuSpacer));
	
	spacer->position = position;
	spacer->width = width;
	spacer->height = height;
	spacer ->bg_color = bg_color;
	spacer->spacing = spacing;
	
	return spacer;
}

void draw_menu_spacer(SDL_Renderer* renderer, MenuSpacer* spacer) {
	// TODO
}

void free_menu_spacer(MenuSpacer* spacer) {
	free(spacer);
}
