#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "linked_list.h"
#include "structs.h"
#include "viewport.h"

typedef enum {
	MENU_NONE,
	MENU_LIST,
	MENU_SELECT,
	MENU_BUTTON,
	MENU_SPACER
} MenuType;

typedef struct menunode {
	Color bg_color;
	VectorD position;
	double width;
	double height;

	MenuType type;
	void* node;
} MenuNode;

typedef struct menulistnode {
	MenuNode* node;
	struct menulistnode* next;
} MenuListNode;

typedef struct menulist {
	double vertical_spacing;
	double outer_spacing;

	int size;
	MenuListNode* first;
} MenuList;

typedef struct menuselect {
	Color text_color;
	char* text;

	bool selected;
} MenuSelect;

typedef struct menubutton {
	Color text_color;
	char* text;
} MenuButton;

typedef struct MenuSpacer {
	double spacing;
} MenuSpacer;

MenuNode* create_menu_root(VectorD position, double width, double height, Color bg_color);

void draw_menu(SDL_Renderer* renderer, Viewport* viewport, MenuNode* menu);
void free_menu_node(MenuNode* node);

MenuList* create_menu_list(double vertical_spacing, double outer_spacing);
void add_menu_list(MenuList* list, MenuNode* node);
MenuNode* remove_menu_list(MenuList* list);
void draw_menu_list(SDL_Renderer* renderer, Viewport* viewport, MenuNode* list);
void free_menu_list(MenuList* list);

MenuSelect* create_menu_select(Color text_color, char* text);
void draw_menu_select(SDL_Renderer* renderer, Viewport* viewport, MenuNode* select);
void free_menu_select(MenuSelect* select);

MenuButton* create_menu_button(Color text_color, char* text);
void draw_menu_button(SDL_Renderer* renderer, Viewport* viewport, MenuNode* button);
void free_menu_button(MenuButton* button);

MenuSpacer* create_menu_spacer(double spacing);
void draw_menu_spacer(SDL_Renderer* renderer, Viewport* viewport, MenuNode* spacer);
void free_menu_spacer(MenuSpacer* spacer);

#endif
