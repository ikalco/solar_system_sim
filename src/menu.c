#include "menu.h"

/*
MenuRoot (MenuList)
	MenuText
	MenuList
		MenuButton
		MenuButton
		MenuButton
	MenuButton
*/

MenuNode* init_menu_sub_list(MenuNode* root_node) {
	Color button_color = (Color){130, 130, 130, 255};
	Color text_color = {235, 235, 235, 255};

	MenuVerticalList* sub_list = create_menu_vlist((Color){50, 50, 50, 240}, (VectorD){5, 5}, 10);
	MenuNode* sub_list_node = create_menu_node((VectorD){0, 0}, (VectorD){MENU_MAX_SIZE, 0}, root_node, MENU_LIST, sub_list);

	MenuButton* button1 = create_menu_button(button_color, text_color, TEXT_CENTER, create_string("Button 1"));
	MenuNode* button1_node = create_menu_node((VectorD){0, 0}, (VectorD){MENU_MAX_SIZE, 60}, sub_list_node, MENU_BUTTON, button1);
	add_menu_vlist(sub_list, button1_node);

	MenuButton* button2 = create_menu_button(button_color, text_color, TEXT_CENTER, create_string("Button 2"));
	MenuNode* button2_node = create_menu_node((VectorD){0, 0}, (VectorD){MENU_MAX_SIZE, 60}, sub_list_node, MENU_BUTTON, button2);
	add_menu_vlist(sub_list, button2_node);

	MenuButton* button3 = create_menu_button(button_color, text_color, TEXT_CENTER, create_string("Button 3"));
	MenuNode* button3_node = create_menu_node((VectorD){0, 0}, (VectorD){MENU_MAX_SIZE, 60}, sub_list_node, MENU_BUTTON, button3);
	add_menu_vlist(sub_list, button3_node);

	return sub_list_node;
}

MenuRoot* init_main_menu(SDL_Window* window) {
	Color text_color = {235, 235, 235, 255};

	MenuVerticalList* root_list = create_menu_vlist((Color){40, 40, 40, 255}, (VectorD){20, 20}, 10);
	MenuNode* root_node = create_menu_node((VectorD){10, 10}, (VectorD){260, 460}, NULL, MENU_LIST, root_list);

	MenuText* title_text = create_menu_text(text_color, TEXT_CENTER, create_string("Main Menu"));
	MenuNode* title_text_node = create_menu_node((VectorD){0, 0}, (VectorD){MENU_MAX_SIZE, 30}, root_node, MENU_TEXT, title_text);
	add_menu_vlist(root_list, title_text_node);

	MenuNode* sub_list_node = init_menu_sub_list(root_node);
	add_menu_vlist(root_list, sub_list_node);

	MenuButton* exit_button = create_menu_button((Color){240, 10, 10, 255}, text_color, TEXT_CENTER, create_string("Exit"));
	MenuNode* exit_button_node = create_menu_node((VectorD){0, 0}, (VectorD){120, 50}, root_node, MENU_BUTTON, exit_button);
	add_menu_vlist(root_list, exit_button_node);

	MenuRoot* root = create_menu_root(window, (VectorD){100, 100}, (VectorD){300, 500}, "fonts/Roboto-Regular.ttf", root_node);

	render_menu_root(root);

	return root;
}

MenuRoot* create_menu_root(SDL_Window* window, VectorD position, VectorD size, const char* fontname, MenuNode* root_node) {
	MenuRoot* root = malloc(sizeof(MenuRoot));

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
	root->menu_texture = SDL_CreateTexture(root->menu_renderer, format, SDL_TEXTUREACCESS_TARGET, size.x, size.y);

	// might want to change this later, but it's supposed to blend alpha which sounds right
	SDL_SetTextureBlendMode(root->menu_texture, SDL_BLENDMODE_BLEND);

	return root;
}

void draw_menu_root(MenuRoot* root) {
	// copy texture to renderer (window)
	SDL_RenderCopy(
		root->menu_renderer,
		root->menu_texture,
		NULL,
		&(SDL_Rect){root->position.x, root->position.y, root->size.x, root->size.y}
	);
}

void free_menu_root(MenuRoot* root) {
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

MenuNode* create_menu_node(VectorD offset, VectorD size, MenuNode* parent, MenuType type, void* node) {
	MenuNode* ret = malloc(sizeof(MenuNode));

	ret->offset = offset;
	ret->size = size;
	ret->type = type;
	ret->node = node;

	ret->parent = parent;
	ret->next = NULL;

	return ret;
}

void free_menu_node(MenuNode* node) {
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

MenuVerticalList* create_menu_vlist(Color bg_color, VectorD padding, double spacing) {
	MenuVerticalList* list = malloc(sizeof(MenuVerticalList));

	list->bg_color = bg_color;
	list->padding = padding;
	list->spacing = spacing;

	list->child = NULL;
	list->size = 0;

	return list;
}

void free_menu_vlist(MenuVerticalList* list) {
	for (MenuNode* current = list->child; current != NULL; current = current->next) {
		MenuNode* next = current->next;
		free_menu_node(current);
		current = next;
	}

	free(list);
}

void add_menu_vlist(MenuVerticalList* list, MenuNode* node) {
	if (list->child == NULL) {
		list->child = node;
		return;
	}

	MenuNode* next = list->child;

	for (int i = 0; i < list->size; i++) {
		next = next->next;
	}

	next->next = node;
	list->size++;
}

MenuText* create_menu_text(Color text_color, MenuTextAlign align, char* text) {
	MenuText* menu_text = malloc(sizeof(MenuText));

	menu_text->text_color = text_color;
	menu_text->text = text;
	menu_text->align = align;

	return menu_text;
}

void free_menu_text(MenuText* text) {
	if (text->text != NULL) {
		free(text->text);
	}

	free(text);
}

MenuButton* create_menu_button(Color bg_color, Color text_color, MenuTextAlign align, char* text) {
	MenuButton* button = malloc(sizeof(MenuButton));

	button->bg_color = bg_color;
	button->text_color = text_color;
	button->text_align = align;
	button->text = text;

	return button;
}

void free_menu_button(MenuButton* button) {
	if (button->text != NULL) {
		free(button->text);
	}

	free(button);
}
