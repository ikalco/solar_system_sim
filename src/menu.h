#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "structs.h"

typedef enum {
	MENU_NONE,
	MENU_ROOT,
	MENU_LIST,
	MENU_TEXT,
	MENU_BUTTON,
	MENU_SPACER
} MenuType;

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
} MenuVerticalList;

typedef struct {
	Color text_color;
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

#endif
