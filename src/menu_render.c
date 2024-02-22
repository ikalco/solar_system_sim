#include "menu.h"

void render_menu_node(MenuRoot* root, MenuNode* node);
void render_menu_vlist(MenuRoot* root, MenuNode* list);
void render_menu_text(MenuRoot* root, MenuNode* text);
void render_menu_button(MenuRoot* root, MenuNode* button);

SDL_Rect get_menu_offset(MenuNode* node) {
	MenuNode* parent = node->parent;

	if (node->size.x == MENU_MAX_SIZE) node->size.x = node->parent->size.x;

	SDL_Rect offset = {node->offset.x, node->offset.y, node->size.x, node->size.y};

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
	switch (node->type) {
		case MENU_NONE:
			break;
		case MENU_LIST:
			render_menu_vlist(root, node);
			break;
		case MENU_TEXT:
			render_menu_text(root, node);
			break;
		case MENU_BUTTON:
			render_menu_button(root, node);
			break;
	}
}

void render_menu_vlist(MenuRoot* root, MenuNode* list_node) {
	if (list_node->type != MENU_LIST)
	{
		printf("Tried to render menu list with a non menu list node\n");
		exit(1);
	}

	MenuVerticalList* list = list_node->node;

	// draw background
	SDL_Rect offset = get_menu_offset(list_node);
	SDL_SetRenderDrawColor(root->menu_renderer, list->bg_color.red, list->bg_color.green, list->bg_color.blue, list->bg_color.alpha);
	SDL_RenderFillRect(root->menu_renderer, &offset);

	// padding
	list_node->offset.x += list->padding.x;
	list_node->offset.y += list->padding.y;
	list_node->size.x -= list->padding.x * 2;
	list_node->size.y -= list->padding.y * 2;

	double spacing_offset = 0;

	// draw children
	for (MenuNode* current = list->child; current != NULL; current = current->next) {
		current->offset.y += spacing_offset;

		render_menu_node(root, current);

		current->offset.y -= spacing_offset;

		spacing_offset += list->spacing + current->size.y;
	}

	// undo padding
	list_node->offset.x -= list->padding.x;
	list_node->offset.y -= list->padding.y;
	list_node->size.x += list->padding.x * 2;
	list_node->size.y += list->padding.y * 2;
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

	SDL_Rect dstrect = get_menu_offset(text_node);
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

void render_menu_button(MenuRoot* root, MenuNode* button_node) {
	if (button_node->type != MENU_BUTTON)
	{
		printf("Tried to render menu text with a non menu text node\n");
		exit(1);
	}

	MenuButton* button = button_node->node;

	// draw background
	SDL_Rect offset = get_menu_offset(button_node);
	SDL_SetRenderDrawColor(root->menu_renderer, button->bg_color.red, button->bg_color.green, button->bg_color.blue, button->bg_color.alpha);
	SDL_RenderFillRect(root->menu_renderer, &offset);

	// type punning here, so don't change size/order of Color
	SDL_Surface* rendered_text = TTF_RenderUTF8_Blended(root->font, button->text, *(SDL_Color*)&button->text_color);

	SDL_Texture* texture = SDL_CreateTextureFromSurface(root->menu_renderer, rendered_text);

	int text_width, text_height;
	TTF_SizeUTF8(root->font, button->text, &text_width, &text_height);

	SDL_Rect dstrect = add_menu_align_offset(offset, button->text_align, text_width, text_height);

	SDL_RenderCopy(
		root->menu_renderer,
		texture,
		NULL,
		&dstrect
	);

	SDL_DestroyTexture(texture);
	SDL_FreeSurface(rendered_text);
}
