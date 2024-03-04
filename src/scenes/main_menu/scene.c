#include "scene.h"
#include "menu.h"
#include "menu/menu.h"
#include "menu/menu_util.h"

void init_main_menu(SceneManager *manager, Scene *scene, SDL_Window *window) {
	Data *data = malloc(sizeof(Data));
	data->manager = manager;
	data->window = window;
	data->root = init_main_menu_root(window);

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

	int clicked_id = menu_has_clicked(menu_data->root, event);

	switch (clicked_id) {
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
}

void draw_main_menu(void *data, SDL_Renderer *renderer) {
	Data *menu_data = data;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	draw_menu_root(menu_data->root);
}
