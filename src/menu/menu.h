#ifndef MENU_H
#define MENU_H

#include "utils/util.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define MENU_MAX_SIZE -1
#define MENU_MIN_SIZE -2

typedef enum {
	MENU_NONE,
	MENU_LIST,
	MENU_TEXT,
	MENU_TEXT_EDIT,
	MENU_BUTTON,
	MENU_LINE_BREAK
} MenuType;

typedef enum { TEXT_LEFT, TEXT_CENTER, TEXT_RIGHT } MenuTextAlign;

typedef enum { MENU_VERTICAL, MENU_HORIZONTAL } MenuDirection;

typedef struct menunode {
	int id;
	SDL_Rect render_offset;
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

	int prev_clicked_id;

	MenuNode *root;
} MenuRoot;

typedef struct {
	Color bg_color;
	VectorD padding;
	double spacing;

	MenuNode *child;
	int size;

	MenuDirection direction;
} MenuList;

typedef struct {
	Color text_color;
	MenuTextAlign align;
	char *text;
} MenuText;

typedef struct {
	Color text_color;
	MenuTextAlign align;
	int selected;
	char *text;
	char *save_text;
} MenuTextEdit;

typedef struct {
	Color bg_color;
	Color text_color;
	MenuTextAlign text_align;
	char *text;
} MenuButton;

// might also make line break have a direction in the future
typedef struct {
	Color bg_color;
} MenuLineBreak;

MenuRoot *create_menu_root(SDL_Window *window,
						   VectorD position,
						   VectorD size,
						   const char *font,
						   MenuNode *root_node);

void render_menu_root(MenuRoot *root);
void draw_menu_root(MenuRoot *root);
void free_menu_root(MenuRoot *root);

MenuNode *create_menu_node(int id,
						   VectorD offset,
						   VectorD size,
						   MenuNode *parent,
						   MenuType type,
						   void *node);
void free_menu_node(MenuNode *node);

MenuList *create_menu_list(Color bg_color,
						   VectorD padding,
						   double spacing,
						   MenuDirection direction);
void free_menu_list(MenuList *list);
void add_menu_list(MenuList *list, MenuNode *node);

MenuText *create_menu_text(Color text_color, MenuTextAlign align, char *text);
void free_menu_text(MenuText *text);

MenuTextEdit *
create_menu_text_edit(Color text_color, MenuTextAlign align, char *text);
void free_menu_text_edit(MenuTextEdit *text);

MenuButton *create_menu_button(Color bg_color,
							   Color text_color,
							   MenuTextAlign align,
							   char *text);
void free_menu_button(MenuButton *button);

MenuLineBreak *create_menu_line_break(Color bg_color);
void free_menu_line_break(MenuLineBreak *line_break);

#endif