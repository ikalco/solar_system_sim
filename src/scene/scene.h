#ifndef SCENE_H
#define SCENE_H

#include <SDL2/SDL.h>

// cleanup should also free (void *data)
typedef struct scene {
	void (*init)(struct scene *scene, SDL_Window *window);
	void (*cleanup)(void *data);
	void (*handle_input)(void *data, SDL_Event *event);
	void (*draw)(void *data, SDL_Renderer *renderer);
	void *data;
} Scene;

Scene *create_scene(
	void (*init)(Scene *scene, SDL_Window *window),
	void (*cleanup)(void *data),
	void (*handle_input)(void *data, SDL_Event *event),
	void (*draw)(void *data, SDL_Renderer *renderer),
	void *data
);

void destroy_scene(Scene *scene);

typedef struct {
	Scene **scenes;
	int num_scenes;
	int capacity;
	int active_index;
} SceneManager;

SceneManager *create_scene_manager(Scene *initial_scene);
void destroy_scene_manager(SceneManager *manager);
void add_scene_manager(SceneManager *manager, Scene *scene);
void remove_scene_manager(SceneManager *manager, int index);
void select_scene(SceneManager *manager, int index);

#endif
