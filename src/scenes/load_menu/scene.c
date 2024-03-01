#include "scene.h"
#include "menu.h"

#include "engine/save.h"
#include "menu/menu.h"
#include "menu/menu_util.h"
#include "options.h"

Scene *create_save_file_scene(char *save_file_name) {
	// TODO
	printf("%s\n", save_file_name);
	return NULL;
}

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

		// we clicked on a save file button
		if (menu_data->clicked_id >= SAVE_BUTTONS) {
			MenuNode *save_file_button_node =
				find_menu_node_id(menu_data->root->root, menu_data->clicked_id);
			MenuButton *save_file_button = save_file_button_node->node;
			char save_file_name[512];

			sprintf(
				save_file_name,
				"%s/%s.save",
				DEFAULT_SAVE_PATH,
				save_file_button->text
			);

			Scene *save_file_scene = create_save_file_scene(save_file_name);

			int scene_i =
				add_scene_manager(menu_data->manager, save_file_scene);

			select_scene_manager(
				menu_data->manager, menu_data->window, scene_i
			);
		}

		menu_data->clicked_id = -1;
	}
}

void draw_load_menu(void *data, SDL_Renderer *renderer) {
	Data *menu_data = data;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	draw_menu_root(menu_data->root);
}
