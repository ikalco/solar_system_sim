#ifndef SCENE_H
#define SCENE_H

#include <SDL2/SDL.h>
#include <stdbool.h>

// make sure this is always correct
#define SCENE_MAIN_MENU_ID 0
#define SCENE_LOAD_MENU_ID 1

struct scene;

typedef struct {
	struct scene **scenes;
	int num_scenes;
	int capacity;
	int active_index;
} SceneManager;

// cleanup should also free (void *data)
typedef struct scene {
	void (*init)(SceneManager *manager,
				 struct scene *scene,
				 SDL_Window *window);
	void (*cleanup)(void *data);
	void (*handle_input)(void *data, SDL_Event *event);
	void (*draw)(void *data, SDL_Renderer *renderer);
	bool initialized;
	void *data;
} Scene;

Scene *create_scene(void (*init)(SceneManager *manager,
								 Scene *scene,
								 SDL_Window *window),
					void (*cleanup)(void *data),
					void (*handle_input)(void *data, SDL_Event *event),
					void (*draw)(void *data, SDL_Renderer *renderer),
					void *data);

void destroy_scene(Scene *scene);

SceneManager *create_scene_manager(Scene *initial_scene);
void destroy_scene_manager(SceneManager *manager);
int add_scene_manager(SceneManager *manager, Scene *scene);
void remove_scene_manager(SceneManager *manager, int index);
void select_scene_manager(SceneManager *manager, SDL_Window *window, int index);

#endif
