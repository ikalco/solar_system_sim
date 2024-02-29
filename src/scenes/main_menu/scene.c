#include "scene.h"
#include "menu.h"
#include "menu/menu.h"
#include "menu/menu_util.h"

void init_main_menu(SceneManager *manager, Scene *scene, SDL_Window *window) {
	Data *data = malloc(sizeof(Data));
	data->manager = manager;
	data->window = window;
	data->root = init_main_menu_root(window);
	data->clicked_id = -1;

	scene->data = data;
}

void cleanup_main_menu(void *data) {
	Data *menu_data = data;

	if (menu_data->root != NULL) {
		free_menu_root(menu_data->root);
	}

	free(menu_data);
}

void handle_input_main_menu(void *data, SDL_Event *event) {
	Data *menu_data = data;

	if (event->type == SDL_MOUSEBUTTONDOWN) {
		menu_data->clicked_id = find_mouse_menu_root(
			menu_data->root, event->button.x, event->button.y
		);
	} else if (event->type == SDL_MOUSEBUTTONUP) {
		int new_clicked_id = find_mouse_menu_root(
			menu_data->root, event->button.x, event->button.y
		);

		// if we clicked something but then dragged mouse off and let go
		// somewhere else, then just don't do anything
		if (menu_data->clicked_id != new_clicked_id) return;

		switch (menu_data->clicked_id) {
		case EXIT_BUTTON:
			exit(0);
		case CREATE_BUTTON:
			break;
		case LOAD_BUTTON:
			select_scene_manager(menu_data->manager, menu_data->window, 1);
			break;
		case SETTINGS_BUTTON:
			break;
		default:
			break;
		}

		menu_data->clicked_id = -1;
	}
}

void draw_main_menu(void *data, SDL_Renderer *renderer) {
	Data *menu_data = data;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	draw_menu_root(menu_data->root);
}
