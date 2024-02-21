#include "menu.h"

void render_menu_node(MenuRoot* root, MenuNode* node);
void render_menu_vlist(MenuRoot* root, MenuNode* list);
void render_menu_text(MenuRoot* root, MenuNode* text);
void render_menu_button(MenuRoot* root, MenuNode* button);

SDL_Rect get_menu_offset(MenuNode *node) {
	// TODO
	return (SDL_Rect){0, 0, 0, 0};
}

SDL_Rect get_menu_align_offset(MenuNode *node, int text_width, int text_height) {
	// TODO
	return (SDL_Rect){0, 0, 0, 0};
}

void render_menu_root(MenuRoot* root) {
	// set renderer to render to our texture
	SDL_SetRenderTarget(root->menu_renderer, root->menu_texture);

	// render menu to the texture
	render_menu_node(root, root->root);

	// set renderer to render back to window
	SDL_SetRenderTarget(root->menu_renderer, NULL);
}

void render_menu_node(MenuRoot* root, MenuNode* node) {
	// TODO
}

void render_menu_vlist(MenuRoot* root, MenuNode* list) {
	// TODO
}

void render_menu_text(MenuRoot* root, MenuNode* text_node) {
	if (text_node->type != MENU_TEXT)
	{
		printf("Tried to render menu text with a non menu text node\n");
		exit(1);
	}

	MenuText* text = text_node->node;

	// type punning here, so don't change size/order of Color
	SDL_Surface* rendered_text = TTF_RenderUTF8_Blended(root->font, text->text, *(SDL_Color*)&text->text_color);

	SDL_Texture* texture = SDL_CreateTextureFromSurface(root->menu_renderer, rendered_text);

	int w, h;
	TTF_SizeUTF8(root->font, text->text, &w, &h);

	SDL_Rect dstrect = get_menu_offset(text_node);
	dstrect = get_menu_align_offset(text_node, w, h);

	SDL_RenderCopy(
		root->menu_renderer,
		texture,
		NULL,
		&dstrect
	);

	SDL_DestroyTexture(texture);
	SDL_FreeSurface(rendered_text);
}

void render_menu_button(MenuRoot* root, MenuNode* button) {
	// TODO
}
