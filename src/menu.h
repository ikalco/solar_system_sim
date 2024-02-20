#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "structs.h"

#define MENU_MAX_SIZE -1
#define MENU_MIN_SIZE -2

typedef enum {
	MENU_NONE,
	MENU_ROOT,
	MENU_LIST,
	MENU_TEXT,
	MENU_BUTTON,
	MENU_SPACER
} MenuType;

typedef enum {
	TEXT_LEFT,
	TEXT_CENTER,
	TEXT_RIGHT
} MenuTextAlign;

typedef struct menunode {
	VectorD offset;
	VectorD size;

	MenuType type;
	void* node;

	struct menunode* next;
} MenuNode;

typedef struct {
	VectorD position;
	VectorD size;

	TTF_Font* font;
	
	MenuNode* root;
} MenuRoot;

typedef struct {
	Color bg_color;
	VectorD padding;
	double spacing;

	MenuNode* child;
	int size;
} MenuVerticalList;

typedef struct {
	Color text_color;
	MenuTextAlign align;
	char* text;
} MenuText;

typedef struct {
	Color text_color;
	Color bg_color;
	char* text;
} MenuButton;

typedef struct {
	double spacing;
} MenuSpacer;

MenuRoot* init_menu();
MenuRoot* create_menu_root(VectorD position, VectorD size, char* font, MenuNode* root_node);

MenuNode* create_menu_node(VectorD offset, VectorD size, MenuType type, void* node);

MenuVerticalList* create_menu_vlist(Color bg_color, VectorD padding, double spacing);
void add_menu_vlist(MenuVerticalList* list, MenuNode* node);

MenuText* create_menu_text(Color text_color, MenuTextAlign align, char* text);

#endif
