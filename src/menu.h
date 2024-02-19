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
	MenuType type;
	void* node;
} MenuNode;

typedef struct menulistnode {
	MenuNode* node;
	struct menulistnode* next;
} MenuListNode;

typedef struct menulist {
	Color bg_color;
	VectorD position;
	double width;
	double height;
	
	double vertical_spacing;
	double outer_spacing;

	int size;
	MenuListNode* first;
} MenuList;

typedef struct menuselect {
	Color bg_color;
	VectorD position;
	double width;
	double height;

	Color text_color;
	char* text;

	bool selected;
} MenuSelect;

typedef struct menubutton {
	Color bg_color;
	VectorD position;
	double width;
	double height;

	Color text_color;
	char* text;
} MenuButton;

typedef struct MenuSpacer {
	Color bg_color;
	VectorD position;
	double width;
	double height;

	double spacing;
} MenuSpacer;

MenuNode* create_menu_root();

void draw_menu_node(SDL_Renderer* renderer, MenuNode* menu);
void free_menu_node(MenuNode* node);

MenuList* create_menu_list(VectorD position, double width, double height, Color bg_color, double vertical_spacing, double outer_spacing);
void add_menu_list(MenuList* list, MenuNode* node);
MenuNode* remove_menu_list(MenuList* list);
void draw_menu_list(SDL_Renderer* renderer, MenuList* list);
void free_menu_list(MenuList* list);

MenuSelect* create_menu_select(VectorD position, double width, double height, Color bg_color, Color text_color, char* text);
void draw_menu_select(SDL_Renderer* renderer, MenuSelect* select);
void free_menu_select(MenuSelect* select);

MenuButton* create_menu_button(VectorD position, double width, double height, Color bg_color, Color text_color, char* text);
void draw_menu_button(SDL_Renderer* renderer, MenuButton* button);
void free_menu_button(MenuButton* button);

MenuSpacer* create_menu_spacer(VectorD position, double width, double height, Color bg_color, double spacing);
void draw_menu_spacer(SDL_Renderer* renderer, MenuSpacer* spacer);
void free_menu_spacer(MenuSpacer* spacer);

#endif
