#include "scene.h"
#include "menu.h"
#include "menu/menu.h"

void init_load_menu(SceneManager *manager, Scene *scene, SDL_Window *window) {
	Data *data = malloc(sizeof(Data));
	data->manager = manager;
	data->window = window;
	data->clicked_id = -1;
	data->num_saves = 0;
	data->root = init_menu_root(window, data);

	scene->data = data;
}

void cleanup_load_menu(void *data) {
	Data *menu_data = data;

	if (menu_data->root != NULL) {
		free_menu_root(menu_data->root);
	}

	free(menu_data);
}

void handle_input_load_menu(void *data, SDL_Event *event) {
	// TODO
}

void draw_load_menu(void *data, SDL_Renderer *renderer) {
	Data *menu_data = data;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	draw_menu_root(menu_data->root);
}
