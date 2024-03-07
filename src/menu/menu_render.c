#include "menu.h"

void render_menu_node(MenuRoot *root, MenuNode *node);
VectorD render_menu_list(MenuRoot *root, MenuNode *list);
VectorD render_menu_text(MenuRoot *root, MenuNode *text);
VectorD render_menu_button(MenuRoot *root, MenuNode *button);
VectorD render_menu_line_break(MenuRoot *root, MenuNode *line_break);

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
	double padding,
	char *text,
	MenuTextAlign align
) {
	if (node->type != MENU_TEXT && node->type != MENU_BUTTON) {
		printf("mistake\n");
	}

	int text_width, text_height;
	TTF_SizeUTF8(root->font, text, &text_width, &text_height);

	offset.w = node->size.x - padding;
	offset.h = node->size.y - padding;

	// make sure text doesn't go outside bounds
	double ratio_width = (offset.h * ((double)text_width / text_height));
	double ratio_height = (offset.w * ((double)text_height / text_width));

	if (node->size.y < node->size.x) {
		if (ratio_width > node->size.x) {
			offset.h = ratio_height;
		} else {
			offset.w = ratio_width;
		}
	} else {
		if (ratio_height > node->size.y) {
			offset.w = ratio_width;
		} else {
			offset.h = ratio_height;
		}
	}

	// always verticaly centered
	offset.y += (node->size.y / 2.0) - (offset.h / 2.0);

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
		node->render_pos = render_menu_list(root, node);
		break;
	case MENU_TEXT:
		node->render_pos = render_menu_text(root, node);
		break;
	case MENU_BUTTON:
		node->render_pos = render_menu_button(root, node);
		break;
	case MENU_LINE_BREAK:
		node->render_pos = render_menu_line_break(root, node);
		break;
	}
}

VectorD render_menu_list(MenuRoot *root, MenuNode *list_node) {
	if (list_node->type != MENU_LIST) {
		printf("Tried to render menu list with a non menu list node\n");
		exit(1);
	}

	MenuList *list = list_node->node;

	int min_height = 0;
	int min_width = 0;
	for (MenuNode *current = list->child; current != NULL;
		 current = current->next) {
		min_height += list->spacing + current->offset.y + current->size.y;
		min_width += list->spacing + current->offset.x + current->size.x;
	}

	if (list_node->size.y < min_height) list_node->size.y = min_height;
	if (list_node->size.x < min_width) list_node->size.x = min_width;

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
	if (list->direction == MENU_VERTICAL) {
		for (MenuNode *current = list->child; current != NULL;
			 current = current->next) {
			current->offset.y += spacing_offset;

			render_menu_node(root, current);

			current->offset.y -= spacing_offset;

			spacing_offset +=
				list->spacing + current->offset.y + current->size.y;
		}
	} else if (list->direction == MENU_HORIZONTAL) {
		for (MenuNode *current = list->child; current != NULL;
			 current = current->next) {
			current->offset.x += spacing_offset;

			render_menu_node(root, current);

			current->offset.x -= spacing_offset;

			spacing_offset +=
				list->spacing + current->offset.x + current->size.x;
		}
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
	SDL_Rect dstrect = get_menu_text_offset(
		root, text_node, offset, 0, text->text, text->align
	);

	SDL_RenderCopy(root->menu_renderer, texture, NULL, &dstrect);

	SDL_DestroyTexture(texture);
	SDL_FreeSurface(rendered_text);

	return (VectorD){offset.x, offset.y};
}

VectorD render_menu_button(MenuRoot *root, MenuNode *button_node) {
	if (button_node->type != MENU_BUTTON) {
		printf("Tried to render menu button with a non menu button node\n");
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
		root, button_node, offset, 10, button->text, button->text_align
	);

	SDL_RenderCopy(root->menu_renderer, texture, NULL, &dstrect);

	SDL_DestroyTexture(texture);
	SDL_FreeSurface(rendered_text);

	return (VectorD){offset.x, offset.y};
}

VectorD render_menu_line_break(MenuRoot *root, MenuNode *line_break_node) {
	if (line_break_node->type != MENU_LINE_BREAK) {
		printf(
			"Tried to render menu line break with a non menu line break node\n"
		);
		exit(1);
	}

	MenuLineBreak *line_break = line_break_node->node;

	if (line_break_node->parent->type == MENU_LIST) {
		MenuList *list = line_break_node->parent->node;
		line_break_node->offset.x = -list->padding.x;
		line_break_node->size.x =
			line_break_node->parent->size.x + list->padding.x * 2;
	}

	// draw background
	SDL_Rect offset = get_menu_offset(line_break_node);
	SDL_SetRenderDrawColor(
		root->menu_renderer,
		line_break->bg_color.red,
		line_break->bg_color.green,
		line_break->bg_color.blue,
		line_break->bg_color.alpha
	);
	SDL_RenderFillRect(root->menu_renderer, &offset);

	return (VectorD){offset.x, offset.y};
}
