#ifndef MENU_H
#define MENU_H

#include "utils/util.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define MENU_MAX_SIZE -1
#define MENU_MIN_SIZE -2

typedef enum { MENU_NONE, MENU_LIST, MENU_TEXT, MENU_BUTTON } MenuType;

typedef enum { TEXT_LEFT, TEXT_CENTER, TEXT_RIGHT } MenuTextAlign;

typedef struct menunode {
	VectorD offset;
	VectorD size;

	MenuType type;
	void *node;

	struct menunode *parent;
	struct menunode *next;
} MenuNode;

typedef struct {
	VectorD position;
	VectorD size;

	SDL_Renderer *menu_renderer;
	SDL_Texture *menu_texture;
	TTF_Font *font;

	MenuNode *root;
} MenuRoot;

typedef struct {
	Color bg_color;
	VectorD padding;
	double spacing;

	MenuNode *child;
	int size;
} MenuVerticalList;

typedef struct {
	Color text_color;
	MenuTextAlign align;
	char *text;
} MenuText;

typedef struct {
	Color bg_color;
	Color text_color;
	MenuTextAlign text_align;
	char *text;
} MenuButton;

MenuRoot *init_main_menu(SDL_Window *window);

MenuRoot *create_menu_root(
	SDL_Window *window,
	VectorD position,
	VectorD size,
	const char *font,
	MenuNode *root_node
);
void render_menu_root(MenuRoot *root);
void draw_menu_root(MenuRoot *root);
void free_menu_root(MenuRoot *root);

MenuNode *create_menu_node(
	VectorD offset,
	VectorD size,
	MenuNode *parent,
	MenuType type,
	void *node
);
void free_menu_node(MenuNode *node);

MenuVerticalList *
create_menu_vlist(Color bg_color, VectorD padding, double spacing);
void free_menu_vlist(MenuVerticalList *list);
void add_menu_vlist(MenuVerticalList *list, MenuNode *node);

MenuText *create_menu_text(Color text_color, MenuTextAlign align, char *text);
void free_menu_text(MenuText *text);

MenuButton *create_menu_button(
	Color bg_color,
	Color text_color,
	MenuTextAlign align,
	char *text
);
void free_menu_button(MenuButton *button);

#endif
