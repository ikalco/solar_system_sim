#include "scene.h"

SceneManager *create_scene_manager() {
	SceneManager *manager = malloc(sizeof(SceneManager));

	manager->scenes = malloc(sizeof(Scene *) * 1);

	manager->capacity = 1;
	manager->num_scenes = 0;
	manager->active_index = -1;

	return manager;
}

void destroy_scene_manager(SceneManager *manager) {
	for (int i = 0; i < manager->num_scenes; i++) {
		if (manager->scenes[i]->initialized) {
			manager->scenes[i]->cleanup(manager->scenes[i]->data);
		}
		free(manager->scenes[i]);
		manager->scenes[i] = NULL;
	}

	free(manager->scenes);
	free(manager);
}

void resize_scene_manager(SceneManager *manager) {
	manager->capacity *= 2;
	manager->scenes =
		realloc(manager->scenes, sizeof(Scene *) * manager->capacity);
	if (manager->scenes == NULL) {
		printf("Something went wrong when resizing scenes in SceneManager");
		exit(1);
	}
}

int add_scene_manager(SceneManager *manager, struct scene scene) {
	if (manager->num_scenes + 1 > manager->capacity)
		resize_scene_manager(manager);

	manager->scenes[manager->num_scenes] = malloc(sizeof(scene));
	memcpy(manager->scenes[manager->num_scenes], &scene, sizeof(scene));
	manager->num_scenes++;

	return manager->num_scenes - 1;
}

void remove_scene_manager(SceneManager *manager, int index) {
	if (index >= manager->num_scenes) {
		printf("Tried to remove invalid scene index from scene manager: %d",
			   index);
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

void select_scene_manager(SceneManager *manager,
						  SDL_Window *window,
						  int index) {
	if (index >= manager->num_scenes) {
		printf("Tried to select invalid scene index from scene manager: %d",
			   index);
		exit(1);
	}

	if (manager->scenes[index]->initialized == false) {
		manager->scenes[index]->init(manager, manager->scenes[index], window);
	}

	manager->active_index = index;
}
