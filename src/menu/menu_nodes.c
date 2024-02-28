#include "menu.h"

MenuRoot *create_menu_root(
	SDL_Window *window,
	VectorD position,
	VectorD size,
	const char *fontname,
	MenuNode *root_node
) {
	MenuRoot *root = malloc(sizeof(MenuRoot));

	root->position = position;
	root->size = size;
	root->root = root_node;

	root->font = TTF_OpenFontDPI(fontname, 12, 1000, 1000);
	if (root->font == NULL) {
		printf("ERROR: %s", SDL_GetError());
		exit(1);
	}

	Uint32 format = SDL_GetWindowPixelFormat(window);
	root->menu_renderer = SDL_GetRenderer(window);
	root->menu_texture = SDL_CreateTexture(
		root->menu_renderer, format, SDL_TEXTUREACCESS_TARGET, size.x, size.y
	);

	// might want to change this later, but it's supposed to blend alpha which
	// sounds right
	SDL_SetTextureBlendMode(root->menu_texture, SDL_BLENDMODE_BLEND);

	return root;
}

void draw_menu_root(MenuRoot *root) {
	// copy texture to renderer (window)
	SDL_RenderCopy(
		root->menu_renderer,
		root->menu_texture,
		NULL,
		&(SDL_Rect
		){root->position.x, root->position.y, root->size.x, root->size.y}
	);
}

void free_menu_root(MenuRoot *root) {
	if (root->root != NULL) {
		free_menu_node(root->root);
	}

	if (root->font != NULL) {
		TTF_CloseFont(root->font);
	}

	if (root->menu_texture != NULL) {
		SDL_DestroyTexture(root->menu_texture);
	}
}

MenuNode *create_menu_node(
	int id,
	VectorD offset,
	VectorD size,
	MenuNode *parent,
	MenuType type,
	void *node
) {
	MenuNode *ret = malloc(sizeof(MenuNode));

	ret->id = id;
	ret->offset = offset;
	ret->size = size;
	ret->type = type;
	ret->node = node;

	ret->parent = parent;
	ret->next = NULL;

	return ret;
}

void free_menu_node(MenuNode *node) {
	if (node->node != NULL) {
		switch (node->type) {
		case MENU_NONE:
			break;
		case MENU_LIST:
			free_menu_node(node->node);
			break;
		case MENU_TEXT:
			free_menu_node(node->node);
			break;
		case MENU_BUTTON:
			free_menu_node(node->node);
			break;
		}
	}

	free(node);
}

MenuVerticalList *
create_menu_vlist(Color bg_color, VectorD padding, double spacing) {
	MenuVerticalList *list = malloc(sizeof(MenuVerticalList));

	list->bg_color = bg_color;
	list->padding = padding;
	list->spacing = spacing;

	list->child = NULL;
	list->size = 0;

	return list;
}

void free_menu_vlist(MenuVerticalList *list) {
	for (MenuNode *current = list->child; current != NULL;
		 current = current->next) {
		MenuNode *next = current->next;
		free_menu_node(current);
		current = next;
	}

	free(list);
}

void add_menu_vlist(MenuVerticalList *list, MenuNode *node) {
	if (list->child == NULL) {
		list->child = node;
		return;
	}

	MenuNode *next = list->child;

	for (int i = 0; i < list->size; i++) {
		next = next->next;
	}

	next->next = node;
	list->size++;
}

MenuText *create_menu_text(Color text_color, MenuTextAlign align, char *text) {
	MenuText *menu_text = malloc(sizeof(MenuText));

	menu_text->text_color = text_color;
	menu_text->text = text;
	menu_text->align = align;

	return menu_text;
}

void free_menu_text(MenuText *text) {
	if (text->text != NULL) {
		free(text->text);
	}

	free(text);
}

MenuButton *create_menu_button(
	Color bg_color,
	Color text_color,
	MenuTextAlign align,
	char *text
) {
	MenuButton *button = malloc(sizeof(MenuButton));

	button->bg_color = bg_color;
	button->text_color = text_color;
	button->text_align = align;
	button->text = text;

	return button;
}

void free_menu_button(MenuButton *button) {
	if (button->text != NULL) {
		free(button->text);
	}

	free(button);
}
