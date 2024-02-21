#include "menu.h"

void render_menu_node(MenuRoot* root, MenuNode* node);
void render_menu_vlist(MenuRoot* root, MenuNode* list);
void render_menu_text(MenuRoot* root, MenuNode* text);
void render_menu_button(MenuRoot* root, MenuNode* button);

SDL_Rect get_menu_offset(MenuRoot* root, MenuNode* node) {
	// we want to add our own offset so that's why we don't set parent here
	MenuNode* parent = node;

	SDL_Rect offset = {root->position.x, root->position.y, node->size.x, node->size.y};

	while (parent != NULL) {
		offset.x += parent->offset.x;
		offset.y += parent->offset.y;
		parent = parent->parent;
	}

	return offset;
}

SDL_Rect add_menu_align_offset(SDL_Rect offset, MenuTextAlign align, int text_width, int text_height) {
	// all alignments are vertically centered, the align is actually for the horizontal
	offset.y += (offset.h / 2) - (text_height / 2);

	switch (align) {
		case TEXT_LEFT:
			break;
		case TEXT_CENTER:
			offset.x += (offset.w / 2) - (text_width / 2);
			break;
		case TEXT_RIGHT:
			offset.x += offset.w - (text_width / 2);
			break;
	}

	return offset;
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

	int text_width, text_height;
	TTF_SizeUTF8(root->font, text->text, &text_width, &text_height);

	SDL_Rect dstrect = get_menu_offset(root, text_node);
	dstrect = add_menu_align_offset(dstrect, text->align, text_width, text_height);

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
