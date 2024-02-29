#include "menu.h"

void render_menu_node(MenuRoot *root, MenuNode *node);
VectorD render_menu_vlist(MenuRoot *root, MenuNode *list);
VectorD render_menu_text(MenuRoot *root, MenuNode *text);
VectorD render_menu_button(MenuRoot *root, MenuNode *button);

SDL_Rect get_menu_offset(MenuNode *node) {
	MenuNode *parent = node->parent;

	if (node->size.x == MENU_MAX_SIZE) node->size.x = node->parent->size.x;

	SDL_Rect offset = {
		node->offset.x, node->offset.y, node->size.x, node->size.y
	};

	while (parent != NULL) {
		offset.x += parent->offset.x;
		offset.y += parent->offset.y;
		parent = parent->parent;
	}

	return offset;
}

SDL_Rect get_menu_text_offset(
	MenuRoot *root,
	MenuNode *node,
	SDL_Rect offset,
	char *text,
	MenuTextAlign align
) {
	if (node->type != MENU_TEXT && node->type != MENU_BUTTON) {
		printf("mistake\n");
	}

	int text_width, text_height;
	TTF_SizeUTF8(root->font, text, &text_width, &text_height);

	offset.w = node->size.x;
	offset.h = node->size.y;

	if (offset.h < offset.w) {
		offset.w = (node->size.y * ((double)text_width / text_height));
	} else {
		offset.h = (node->size.x * ((double)text_height / text_width));
	}

	switch (align) {
	case TEXT_LEFT:
		break;
	case TEXT_CENTER:
		offset.x += (node->size.x / 2.0) - (offset.w / 2.0);
		break;
	case TEXT_RIGHT:
		offset.x += node->size.x - (offset.w / 2.0);
		break;
	}

	return offset;
}

void render_menu_root(MenuRoot *root) {
	// set renderer to render to our texture
	SDL_SetRenderTarget(root->menu_renderer, root->menu_texture);

	// render menu to the texture
	render_menu_node(root, root->root);

	// set renderer to render back to window
	SDL_SetRenderTarget(root->menu_renderer, NULL);
}

void render_menu_node(MenuRoot *root, MenuNode *node) {
	switch (node->type) {
	case MENU_NONE:
		break;
	case MENU_LIST:
		node->render_pos = render_menu_vlist(root, node);
		break;
	case MENU_TEXT:
		node->render_pos = render_menu_text(root, node);
		break;
	case MENU_BUTTON:
		node->render_pos = render_menu_button(root, node);
		break;
	}
}

VectorD render_menu_vlist(MenuRoot *root, MenuNode *list_node) {
	if (list_node->type != MENU_LIST) {
		printf("Tried to render menu list with a non menu list node\n");
		exit(1);
	}

	MenuVerticalList *list = list_node->node;

	int min_height = 0;
	for (MenuNode *current = list->child; current != NULL;
		 current = current->next) {
		min_height += list->spacing + current->offset.y + current->size.y;
	}

	if (list_node->size.y < min_height) list_node->size.y = min_height;

	// draw background
	SDL_Rect offset = get_menu_offset(list_node);
	SDL_SetRenderDrawColor(
		root->menu_renderer,
		list->bg_color.red,
		list->bg_color.green,
		list->bg_color.blue,
		list->bg_color.alpha
	);
	SDL_RenderFillRect(root->menu_renderer, &offset);

	// padding
	list_node->offset.x += list->padding.x;
	list_node->offset.y += list->padding.y;
	list_node->size.x -= list->padding.x * 2;
	list_node->size.y -= list->padding.y * 2;

	double spacing_offset = 0;

	// draw children
	for (MenuNode *current = list->child; current != NULL;
		 current = current->next) {
		current->offset.y += spacing_offset;

		render_menu_node(root, current);

		current->offset.y -= spacing_offset;

		spacing_offset += list->spacing + current->offset.y + current->size.y;
	}

	// undo padding
	list_node->offset.x -= list->padding.x;
	list_node->offset.y -= list->padding.y;
	list_node->size.x += list->padding.x * 2;
	list_node->size.y += list->padding.y * 2;

	return (VectorD){offset.x, offset.y};
}

VectorD render_menu_text(MenuRoot *root, MenuNode *text_node) {
	if (text_node->type != MENU_TEXT) {
		printf("Tried to render menu text with a non menu text node\n");
		exit(1);
	}

	MenuText *text = text_node->node;

	// type punning here, so don't change size/order of Color
	SDL_Surface *rendered_text = TTF_RenderUTF8_Blended(
		root->font, text->text, *(SDL_Color *)&text->text_color
	);

	SDL_Texture *texture =
		SDL_CreateTextureFromSurface(root->menu_renderer, rendered_text);

	SDL_Rect offset = get_menu_offset(text_node);
	SDL_Rect dstrect =
		get_menu_text_offset(root, text_node, offset, text->text, text->align);

	SDL_RenderCopy(root->menu_renderer, texture, NULL, &dstrect);

	SDL_DestroyTexture(texture);
	SDL_FreeSurface(rendered_text);

	return (VectorD){offset.x, offset.y};
}

VectorD render_menu_button(MenuRoot *root, MenuNode *button_node) {
	if (button_node->type != MENU_BUTTON) {
		printf("Tried to render menu text with a non menu text node\n");
		exit(1);
	}

	MenuButton *button = button_node->node;

	// draw background
	SDL_Rect offset = get_menu_offset(button_node);
	SDL_SetRenderDrawColor(
		root->menu_renderer,
		button->bg_color.red,
		button->bg_color.green,
		button->bg_color.blue,
		button->bg_color.alpha
	);
	SDL_RenderFillRect(root->menu_renderer, &offset);

	// type punning here, so don't change size/order of Color
	SDL_Surface *rendered_text = TTF_RenderUTF8_Blended(
		root->font, button->text, *(SDL_Color *)&button->text_color
	);

	SDL_Texture *texture =
		SDL_CreateTextureFromSurface(root->menu_renderer, rendered_text);

	SDL_Rect dstrect = get_menu_text_offset(
		root, button_node, offset, button->text, button->text_align
	);

	SDL_RenderCopy(root->menu_renderer, texture, NULL, &dstrect);

	SDL_DestroyTexture(texture);
	SDL_FreeSurface(rendered_text);

	return (VectorD){offset.x, offset.y};
}
