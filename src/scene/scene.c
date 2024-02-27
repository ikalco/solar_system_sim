#include "scene.h"

Scene *create_scene(
	void (*init)(void *data, SDL_Window *window),
	void (*cleanup)(void *data),
	void (*handle_input)(void *data, SDL_Event event),
	void (*draw)(void *data, SDL_Renderer *renderer),
	void *data
) {
	Scene *scene = malloc(sizeof(Scene));

	scene->init = init;
	scene->cleanup = cleanup;
	scene->handle_input = handle_input;
	scene->draw = draw;
	scene->data = data;

	return scene;
}

SceneManager *create_scene_manager(Scene *initial_scene) {
	SceneManager *manager = malloc(sizeof(SceneManager));

	manager->scenes = malloc(sizeof(Scene *) * 1);
	manager->scenes[0] = initial_scene;

	manager->capacity = 1;
	manager->num_scenes = 1;
	manager->active_index = 0;

	return manager;
}

void destroy_scene_manager(SceneManager *manager) {
	for (int i = 0; i < manager->num_scenes; i++) {
		manager->scenes[i]->cleanup(manager->scenes[i]->data);
		free(manager->scenes[i]);
		manager->scenes[i] = NULL;
	}

	free(manager->scenes);
	free(manager);
}

void resize_scene_manager(SceneManager* manager) {
	manager->capacity *= 2;
	manager->scenes = realloc(manager->scenes, sizeof(Scene*) * manager->capacity);
	if (manager->scenes == NULL) {
		printf("Something went wrong when resizing scenes in SceneManager");
		exit(1);
	}
}

void add_scene_manager(SceneManager *manager, Scene *scene) {
	if (manager->num_scenes + 1 > manager->capacity)
		resize_scene_manager(manager);

	manager->scenes[manager->num_scenes] = scene;
	manager->num_scenes++;
}

void remove_scene_manager(SceneManager *manager, int index) {
	if (index >= manager->num_scenes) {
		printf(
			"Tried to remove invalid scene index from scene manager: %d", index
		);
		exit(1);
	}

	manager->scenes[index]->cleanup(manager->scenes[index]->data);
	free(manager->scenes[index]);
	manager->scenes[index] = NULL;

	for (int i = index; i < manager->num_scenes - 1; i++) {
		manager->scenes[i] = manager->scenes[i + 1];
	}

	manager->scenes[manager->num_scenes - 1] = NULL;
	manager->num_scenes--;
}

void select_scene(SceneManager *manager, int index) {
	if (index >= manager->num_scenes) {
		printf(
			"Tried to select invalid scene index from scene manager: %d", index
		);
		exit(1);
	}

	manager->active_index = index;
}
