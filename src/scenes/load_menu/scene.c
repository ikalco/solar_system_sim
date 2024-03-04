#include "scene.h"
#include "menu.h"

#include "menu/menu.h"
#include "menu/menu_util.h"
#include "options.h"

#include "scenes/solar_system/scene.h"

Scene *create_save_file_scene(Data *data, char *save_file_name) {
	Scene *solar_system_scene = create_scene(
		init_solar_system,
		cleanup_solar_system,
		handle_input_solar_system,
		draw_solar_system,
		save_file_name
	);

	return solar_system_scene;
}

void init_load_menu(SceneManager *manager, Scene *scene, SDL_Window *window) {
	Data *data = malloc(sizeof(Data));
	data->manager = manager;
	data->window = window;
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
	Data *menu_data = data;

	int clicked_id = menu_has_clicked(menu_data->root, event);

	if (clicked_id >= SAVE_BUTTONS) {
		MenuNode *save_file_button_node =
			find_menu_node_id(menu_data->root->root, clicked_id);
		MenuButton *save_file_button = save_file_button_node->node;
		char save_file_name[512];

		sprintf(
			save_file_name,
			"%s/%s.save",
			DEFAULT_SAVE_PATH,
			save_file_button->text
		);

		Scene *save_file_scene =
			create_save_file_scene(data, create_string(save_file_name));

		int scene_i = add_scene_manager(menu_data->manager, save_file_scene);

		select_scene_manager(menu_data->manager, menu_data->window, scene_i);
	}
}

void draw_load_menu(void *data, SDL_Renderer *renderer) {
	Data *menu_data = data;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	draw_menu_root(menu_data->root);
}
