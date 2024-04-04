#include "menu_text_edit.h"
#include "menu_util.h"

SDL_Rect render_menu_text_edit(MenuRoot *root, MenuNode *text_node) {
	if (text_node->type != MENU_TEXT_EDIT) {
		printf("Tried to render menu text with a non menu text edit node\n");
		exit(1);
	}

	MenuTextEdit *text = text_node->node;

	// type punning here, so don't change size/order of Color
	SDL_Surface *rendered_text = TTF_RenderUTF8_Blended(
		root->font, text->text, *(SDL_Color *)&text->text_color);

	SDL_Texture *texture =
		SDL_CreateTextureFromSurface(root->menu_renderer, rendered_text);

	SDL_Rect offset = get_menu_offset(text_node);
	SDL_Rect textrect = get_menu_text_offset(
		root, text_node, (SDL_Rect){0, 0, 0, 0}, 0, text->text, text->align);

	SDL_Rect dstrect;

	SDL_SetRenderDrawColor(root->menu_renderer, 235, 235, 235, 255);

	if (text->selected) {
		dstrect = (SDL_Rect){.x = offset.x + offset.w - textrect.x,
							 .y = offset.y + textrect.y,
							 .w = 15,
							 .h = textrect.h};

		SDL_RenderFillRect(root->menu_renderer, &dstrect);
	}

	dstrect = (SDL_Rect){.x = offset.x + textrect.x,
						 .y = offset.y + textrect.y,
						 .w = textrect.w,
						 .h = textrect.h};

	SDL_RenderCopy(root->menu_renderer, texture, NULL, &dstrect);

	if (text->selected) dstrect.w += 15;
	dstrect.x -= 5;
	dstrect.y -= 3;
	dstrect.w += 10;
	dstrect.h += 6;
	SDL_RenderDrawRect(root->menu_renderer, &dstrect);

	SDL_DestroyTexture(texture);
	SDL_FreeSurface(rendered_text);

	return offset;
}

void menu_text_edit_stop_edit(MenuRoot *root, MenuTextEdit *text) {
	SDL_StopTextInput();
	text->selected = 0;
	if (text->save_text != NULL) free(text->save_text);
	text->save_text = NULL;
	render_menu_root(root);
}

void menu_text_edit_start_edit(MenuRoot *root, MenuTextEdit *text) {
	text->save_text = malloc(sizeof(char[MENU_TEXT_EDIT_SIZE]));
	strncpy(text->save_text, text->text, MENU_TEXT_EDIT_SIZE);
	text->selected = 1;
	SDL_StartTextInput();
	render_menu_root(root);
}

void menu_text_edit_handle_events(MenuRoot *root,
								  MenuTextEdit *text,
								  SDL_Event *event) {
	if (!text->selected) return;

	if (event->type == SDL_KEYDOWN) {
		switch (event->key.keysym.sym) {
		case SDLK_RETURN:
			// save changed text
			menu_text_edit_stop_edit(root, text);
			break;
		case SDLK_ESCAPE:
			// discard changed text
			strncpy(text->text, text->save_text, MENU_TEXT_EDIT_SIZE);
			menu_text_edit_stop_edit(root, text);
			break;
		case SDLK_BACKSPACE:
			// delete character at cursor
			int len = strlen(text->text);
			if (len == 0) return;
			text->text[len - 1] = 0;

			break;
		}

		render_menu_root(root);
	}

	if (event->type == SDL_TEXTINPUT) {
		if (strlen(text->text) + strlen(event->text.text) >=
			MENU_TEXT_EDIT_SIZE)
			return;
		strcat(text->text, event->text.text);
		render_menu_root(root);
	}
}