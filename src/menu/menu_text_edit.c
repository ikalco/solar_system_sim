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
		root->font, text->text, *(SDL_Color *)&text->text_color
	);

	SDL_Texture *texture
		= SDL_CreateTextureFromSurface(root->menu_renderer, rendered_text);

	SDL_Rect offset = get_menu_offset(text_node);
	SDL_Rect textrect = get_menu_text_offset(
		root, text_node, (SDL_Rect){0, 0, 0, 0}, 0, text->text, text->align
	);

	SDL_Rect dstrect;

	if (text->selected) {
		dstrect = (SDL_Rect
		){offset.x + offset.w - textrect.x,
		  offset.y + textrect.y,
		  15,
		  textrect.h};
		SDL_SetRenderDrawColor(root->menu_renderer, 235, 235, 235, 255);
		SDL_RenderFillRect(root->menu_renderer, &dstrect);
	}

	dstrect = (SDL_Rect
	){offset.x + textrect.x, offset.y + textrect.y, textrect.w, textrect.h};

	SDL_RenderCopy(root->menu_renderer, texture, NULL, &dstrect);

	SDL_DestroyTexture(texture);
	SDL_FreeSurface(rendered_text);

	return offset;
}

void menu_text_edit_stop_edit(MenuRoot *root, MenuTextEdit *text) {
	SDL_StopTextInput();
	text->selected = 1;
	if (text->save_text != NULL) free(text->save_text);
	render_menu_root(root);
}

void menu_text_edit_start_edit(MenuRoot *root, MenuTextEdit *text) {
	text->save_text = create_string(text->text);
	text->selected = 1;
	SDL_StartTextInput();
	render_menu_root(root);
}

void menu_text_edit_handle_events(
	MenuRoot *root,
	MenuTextEdit *text,
	SDL_Event *event
) {
	if (!text->selected) return;

	if (event->type == SDL_KEYDOWN) {
		switch (event->key.keysym.sym) {
		case SDLK_RETURN:
			// save changed text
			break;
		case SDLK_ESCAPE:
			// discard changed text
			break;
		case SDLK_LEFT:
			// move cursor left
			break;
		case SDLK_RIGHT:
			// move cursor right
			break;
		case SDLK_BACKSPACE:
			// delete character at cursor
			break;
		}
	}

	if (event->type == SDL_TEXTINPUT) {
		strcat(text->text, event->text.text);
		render_menu_root(root);
	}
}