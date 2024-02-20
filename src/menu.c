#include "menu.h"

MenuRoot* create_menu_root(VectorD position, VectorD size, char* fontname, MenuNode* root_node) {
	MenuRoot* root = malloc(sizeof(MenuRoot));

	root->position = position;
	root->size = size;
	root->root = root_node;
	root->fontname = fontname;

	return root;
}

void draw_menu_root(SDL_Renderer* renderer, MenuRoot* root) {

}

void free_menu_root(MenuRoot* root) {

}
