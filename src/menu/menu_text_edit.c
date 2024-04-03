#include "menu_text_edit.h"
#include "menu_util.h"

SDL_Rect render_menu_text_edit(MenuRoot *root, MenuNode *text);
void menu_text_edit_handle_events(MenuText *text, SDL_Event *event);
void menu_text_edit_start_edit(MenuText *text);
void menu_text_edit_stop_edit(MenuText *text);