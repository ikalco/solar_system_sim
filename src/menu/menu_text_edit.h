#ifndef MENU_TEXT_EDIT_H
#define MENU_TEXT_EDIT_H

#include "menu.h"

SDL_Rect render_menu_text_edit(MenuRoot *root, MenuNode *text);
void menu_text_edit_handle_events(
	MenuRoot *root,
	MenuTextEdit *text,
	SDL_Event *event
);
void menu_text_edit_start_edit(MenuRoot *root, MenuTextEdit *text);
void menu_text_edit_stop_edit(MenuRoot *root, MenuTextEdit *text);

#endif