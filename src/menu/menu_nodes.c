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
	root->prev_clicked_id = -1;

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
	ret->render_offset = (SDL_Rect){0, 0, 0, 0};
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
			free_menu_list(node->node);
			break;
		case MENU_TEXT:
			free_menu_text(node->node);
			break;
		case MENU_BUTTON:
			free_menu_button(node->node);
			break;
		case MENU_LINE_BREAK:
			free_menu_line_break(node->node);
			break;
		}
	}

	free(node);
}

MenuList *create_menu_list(
	Color bg_color,
	VectorD padding,
	double spacing,
	MenuDirection direction
) {
	MenuList *list = malloc(sizeof(MenuList));

	list->bg_color = bg_color;
	list->padding = padding;
	list->spacing = spacing;
	list->direction = direction;

	list->child = NULL;
	list->size = 0;

	return list;
}

void free_menu_list(MenuList *list) {
	MenuNode *current = list->child;
	while (current != NULL) {
		MenuNode *next = current->next;
		free_menu_node(current);
		current = next;
	}

	free(list);
}

void add_menu_list(MenuList *list, MenuNode *node) {
	if (list->child == NULL) {
		list->child = node;
		list->size++;
		return;
	}

	MenuNode *next = list->child;

	for (int i = 1; i < list->size; i++) {
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

MenuLineBreak *create_menu_line_break(Color bg_color) {
	MenuLineBreak *line_break = malloc(sizeof(MenuLineBreak));

	line_break->bg_color = bg_color;

	return line_break;
}

void free_menu_line_break(MenuLineBreak *line_break) { free(line_break); }
